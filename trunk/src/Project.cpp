#include "Project.h"
#include "MyAssert.h"
#include "SDL_image.h"
#include "MyStringUtils.h"
#include <vector>
#include <string>
#include "Debug.h"
#include "Bitmap.h"
#include "Errors.h"
#include <SDL/SDL_rotozoom.h>

void BitmapFitOptimizerFunction::setBitmaps(Bitmap* b1,
        Bitmap* b2) {
    this->b1 = b1;
    this->b2 = b2;
}

double BitmapFitOptimizerFunction::evaluate(std::vector<double>
        values){
    Assert(values.size()==ColorAdjust::CNT, "");
    Assert(b1,"");
    Assert(b2,"");

    adjust.variables=values;
    return  distance.distance(b1,b2,&adjust)/10000.0; // normalizujme
}

Project::Project(Cache* cache){
    outputSurface=NULL;
  this->cache = cache;

  std::vector<double> min(ColorAdjust::min,
          ColorAdjust::min+ColorAdjust::CNT);
  std::vector<double> max(ColorAdjust::max,
          ColorAdjust::max+ColorAdjust::CNT);

  optimizer.initialize(min,max, &optimizerFunction);
}

Project::~Project(){
  clean();
}

void Project::clean(){
  SDL_FreeSurface(outputSurface);
}

void Project::loadImageStack(){
    DEBUG("load image stack");

    SDL_Surface* tmpSurface =  IMG_Load(inputImageFile.c_str());
    Assert(tmpSurface, "IMG_Load failed");
    SDL_Surface* inputSurface = SDL_DisplayFormat(tmpSurface);
    Assert(inputSurface, "IMG_Load failed");
    SDL_FreeSurface(tmpSurface);

    thumbnailMatrix.initializeMatrix(inputSurface, tileDivision, tilesX,
            tilesY, outputSurface, outputDivision);

    SDL_FreeSurface(inputSurface);
}

void Project::loadMapping(){
    DEBUG("loading mapping and placing images");
    for (int x = 0; x < tilesX; x++) {
        printf("loading mapping %d / %d\n",x,tilesX);
        for (int y = 0; y < tilesY; y++) {
            std::string hash_key =
                thumbnailMatrix.getMappingString(x,y)+"/hash";
            std::string adj_key =
                thumbnailMatrix.getMappingString(x,y)+"/adjust";
            std::string hash = thumbnailMapping.getDefaultString(
                                hash_key, "__nothing");

            CacheData cdata;

            cdata = cache->getCacheData(hash);

            ThumbnailStack stack;
            stack.loadFromCache(cdata, 0);
            ColorAdjust adjust;
            adjust.fromString(thumbnailMapping.getDefaultString(adj_key,
                        ""));

            thumbnailMatrix.placeImageAt(hash, &stack, &adjust, x,y);
            progressStatistics.nextImage(thumbnailMatrix.getData(x,y).fitData.diff);
        }
    }
    progressStatistics.clearIncremental();
    DEBUG("done");
}

void Project::createOutputSurface(){
    int outputWidth = tilesX * Cache::getTileWidth(outputDivision);
    int outputHeight = tilesY * Cache::getTileHeight(outputDivision);

    outputSurface=SDL_CreateRGBSurface(SDL_SWSURFACE,
            outputWidth, outputHeight,
            32, 0, 0, 0, 0);

    Assert(outputSurface, "SDL_CreateRGBSurface failed");
    DEBUG("output surface created");

}

void Project::load(const std::string& dir,
        const std::string& file) {
    clean();
    Errors::_addError("loading project", Errors::NOTICE);

    projectFile = dir + "/" + file;
    preferences.loadFromFile(projectFile);

    skipFile = dir + "/" + preferences.getString("/file/skip");
    inputImageFile = dir + "/" + preferences.getString("/file/input");
    outputImageFile = dir + "/" + preferences.getString("/file/output");
    mappingFile = dir + "/" + preferences.getString("/file/mapping");

    skipImages.loadFromFile(skipFile);
    thumbnailMapping.loadFromFile(mappingFile);

    tilesX = preferences.getInt("/mosaic/tilesX");
    tilesY = preferences.getInt("/mosaic/tilesY");

    tileDivision =
        preferences.getInt("/image/thumbnails/subdivision");
    outputDivision =
        preferences.getInt("/image/thumbnails/output_division");

    thumbnailMatrix.initialize(&thumbnailMapping, &skipImages, 
            preferences.getInt("/mosaic/cnt_same_image"));

    createOutputSurface();
    loadImageStack();
    loadMapping();

    cacheEnumeration = cache->enumerate();
    Assert(cacheEnumeration.size(),"");
    random_shuffle(cacheEnumeration.begin(), cacheEnumeration.end());
    cacheEnumerationId = 0;
    saveOutput();
}

void Project::saveOutput(){
    DEBUG("saving output...");
    Assert(outputSurface, "No output surface");

    SDL_SaveBMP(outputSurface, outputImageFile.c_str());

    DEBUG("blitting to window");
    SDL_Surface* videoSurface = SDL_GetVideoSurface();
    Assert(videoSurface, "can't get video surface");
    double zoomX =  (double)videoSurface->w/outputSurface->w;
    double zoomY =  (double)videoSurface->h/outputSurface->h;
    double zoom = std::min(zoomX, zoomY);

    SDL_Surface* tmpSurface =
        zoomSurface(outputSurface,zoom,zoom,0);
    Assert(tmpSurface, "can't create surface");

    Assert(SDL_BlitSurface(tmpSurface,NULL,videoSurface,NULL)==0,
            "problem blitting");
    SDL_UpdateRect(videoSurface, 0, 0, 0, 0);

    SDL_FreeSurface(tmpSurface);
}

void Project::showStatistics(){
    thumbnailMatrix.saveFitData("statistics.txt");

    DEBUG(progressStatistics.getStatistics());
    DEBUG("estimated final value " +
            MyStringUtils::doubleToString(
                progressStatistics.estimateFinalDiff(
                    cache->getSize()-skipImages.size())));
    progressStatistics.clearIncremental();

    thumbnailMatrix.recomputeSubdivisionFitCutoffs();

    DEBUG("Optimizer iterations average: "+
            MyStringUtils::doubleToString(optimizer.averageIterations()));
}

void Project::save(){
    DEBUG("saving...");
    preferences.saveToFile(projectFile);
    skipImages.saveToFile(skipFile);
    thumbnailMapping.saveToFile(mappingFile);
}


void Project::fitNextImage(){
    int id=cacheEnumerationId;
    int internalCnt=0;
    Assert(cacheEnumeration.size(), "empy cache enum");

    while (!thumbnailMatrix.canPlaceThumbnail(
                cacheEnumeration[id])) {
        id = (id+1) % cacheEnumeration.size();
        internalCnt++;
        Assert(internalCnt <= cacheEnumeration.size(),
                "skipping whole cache");
    }

    std::string hash = cacheEnumeration[id];

    CacheData cdata;

    cdata = cache->getCacheData(hash);

    ThumbnailStack stack;
    stack.loadFromCache(cdata, 0);

    double best=0;
    ColorAdjust bestAdjust;
    ColorAdjust noAdjust;
    int bx=-1;
    int by=-1;

    for (int x = 0; x < tilesX; x++)
        for (int y = 0; y < tilesY; y++) {
            int ok=1;
            ColorAdjust adjust;

            for (int subdiv=ThumbnailStack::SIZE-1;
                    subdiv>=tileDivision; subdiv--) {

                if (subdiv==ThumbnailStack::SIZE-1) {
                    double fit = thumbnailMatrix.getImageFit(&stack,
                            &noAdjust,
                            subdiv, x, y);

                    fit -= thumbnailMatrix.getData(x,y).fitData.diff;
                    fit += thumbnailMatrix.getSubdivisionFitCutoffOriginal(subdiv);
                    if (fit>best) {
                        ok=0;
                        break;
                    }
                }

                //                printf("optimize subdiv %d\n",subdiv);
                optimizerFunction.setBitmaps(&stack.stack[subdiv],
                        &thumbnailMatrix.getData(x,y).stackData.stack[subdiv]);

                int iter = 2 + (ThumbnailStack::SIZE - subdiv)*2;
                adjust.variables = optimizer.optimize(adjust.variables,
                        0.0005, iter);

                double fit = thumbnailMatrix.getImageFit(&stack,
                        &adjust,
                        subdiv, x, y);

                fit -= thumbnailMatrix.getData(x,y).fitData.diff;
                fit += thumbnailMatrix.getSubdivisionFitCutoff(subdiv);
                if (fit>best) {
                    ok=0;
                    break;
                }
            }

            if (ok) {
            double fit = thumbnailMatrix.getImageFit(&stack, &adjust,
                    tileDivision, x, y);

            fit -= thumbnailMatrix.getData(x,y).fitData.diff;

            if (fit<best) {
                best=fit;
                bx=x;
                by=y;
                bestAdjust=adjust;
            }
            }
        }

    if (best<0) {
        thumbnailMatrix.placeImageAt(hash, &stack, &bestAdjust, bx, by);
    } else {
        skipImages.setString(hash,"no_fit");
    }

    progressStatistics.nextImage(best);

    //printf("best fit %8.2f at %d %d\n",best,bx,by);
    
    cacheEnumerationId = id;
}
