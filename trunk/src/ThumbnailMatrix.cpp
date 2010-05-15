// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "ThumbnailMatrix.h"
#include "Cache.h"
#include "MyStringUtils.h"
#include "MyAssert.h"
#include "Debug.h"
#include <SDL/SDL.h>
#include <fstream>
#include <algorithm>

bool ThumbnailMatrix::canPlaceThumbnail(std::string hash) {
  if (skip->haveKey(hash)) return false;
  if (imageCount[hash] >= sameImageMax) return false;
  return true;
}

double ThumbnailMatrix::getImageFit(ThumbnailStack* stack,
        ColorAdjust* adjust, int subdiv,
        int x, int y) {
    return distanceFunction.distance(
            &matrix[x][y].stackData.stack[subdiv],
            &(stack->stack[subdiv]),
            adjust);
}

std::string ThumbnailMatrix::getCellSummary(int x,int y){
    if (x>=width || y>=height) return "~~~~";

 std::string summary;
 summary = "Summary info for ";
 summary+= MyStringUtils::intToString(x)+","+MyStringUtils::intToString(y);
 summary+= " hash:";
 summary+= matrix[x][y].fitData.thumbnailHash;
 summary+= " adjust:";
 summary+= matrix[x][y].fitData.adjust.toString();
 summary+= " fit data:";
 summary+= matrix[x][y].fitData.getFitStatistics();
 return summary;
}

void ThumbnailMatrix::placeImageAt(std::string& hash, 
        ThumbnailStack* stack, ColorAdjust* adjust, int x, int y) {

    ThumbnailFitData& fitData=matrix[x][y].fitData;
    imageCount[fitData.thumbnailHash]--;
    fitData.thumbnailHash = hash;
    imageCount[hash]++;

    ColorAdjust adjustOrig;

    for (int subdiv = subdivision; subdiv < ThumbnailStack::SIZE;
            subdiv++) {
        double diff = getImageFit(stack, adjust, subdiv, x, y);
        double diffOrig = getImageFit(stack, &adjustOrig, subdiv, x, y);
        fitData.diffStack[subdiv] = diff;
        fitData.diffStackOriginal[subdiv] = diffOrig;
    }
    fitData.diff = fitData.diffStack[subdivision];
    fitData.adjust = *adjust;
    stack->stack[outputDivision].blitToSurface(outputSurface,
            adjust,
            x * Cache::getTileWidth(outputDivision),
            y * Cache::getTileHeight(outputDivision));

    mapping->setString(getMappingString(x,y)+"/hash", hash);
    mapping->setString(getMappingString(x,y)+"/adjust",
            adjust->toString());
}

double ThumbnailMatrix::getSubdivisionFitCutoff(int subdiv){
  Assert(subdiv < subdivisionFitCutoff.size(), "");
  return subdivisionFitCutoff[subdiv];
}

double ThumbnailMatrix::getSubdivisionFitCutoffOriginal(int subdiv){
  Assert(subdiv < subdivisionFitCutoffOriginal.size(), "");
  return subdivisionFitCutoffOriginal[subdiv];
}

void ThumbnailMatrix::recomputeSubdivisionFitCutoffs(){
  for (int subdiv=subdivision; subdiv<subdivisionFitCutoff.size(); subdiv++) {

      std::vector<double> tmp;

      for (int x=0; x<width; x++)
          for (int y=0; y<height; y++)
              tmp.push_back(matrix[x][y].fitData.diff -
                      matrix[x][y].fitData.diffStack[subdiv]
                      );

    sort(tmp.begin(), tmp.end());
    // remove 2 worst points
    double value=tmp[2];
    if (subdiv) {
        value = std::min(value, subdivisionFitCutoff[subdiv-1]);
    }

    
    subdivisionFitCutoff[subdiv] = value;
    printf("Subdivision fit cutoff  %d = %.2f\n", subdiv,
            subdivisionFitCutoff[subdiv]);
  }


  for (int subdiv=subdivision; subdiv<subdivisionFitCutoff.size(); subdiv++) {

      std::vector<double> tmp;

      for (int x=0; x<width; x++)
          for (int y=0; y<height; y++)
              tmp.push_back(matrix[x][y].fitData.diff -
                      matrix[x][y].fitData.diffStackOriginal[subdiv]
                      );

    sort(tmp.begin(), tmp.end());
    // remove 2 worst points
    double value=tmp[2];
    if (subdiv) {
        value = std::min(value, subdivisionFitCutoffOriginal[subdiv-1]);
    }

    subdivisionFitCutoffOriginal[subdiv] = value;
    printf("Subdivision fit cutoff original %d = %.2f\n", subdiv,
            subdivisionFitCutoffOriginal[subdiv]);
  }
}

void ThumbnailMatrix::saveFitData(const std::string& filename){
    std::ofstream f;
    f.open(filename.c_str());
    Assert(!f.fail(), "failed to write statistics");

    for (int x=0; x < width; x++)
        for (int y=0; y<height; y++)
            f<<matrix[x][y].fitData.getFitStatistics()<<std::endl;

    f.close();
}

void ThumbnailMatrix::initialize(Preferences* mapping, Preferences*
        skip, int sameImageMax) {
    this->mapping = mapping;
    this->skip = skip;
    this->sameImageMax = sameImageMax;
}

std::string ThumbnailMatrix::getMappingString(int x,int y){
  return "x_"+MyStringUtils::intToString(x)+"/y_"+MyStringUtils::intToString(y);
}

ThumbnailMatrixData& ThumbnailMatrix::getData(int x,int y){
    return matrix[x][y];
}

void ThumbnailMatrix::initializeMatrix(SDL_Surface* surface, int
        subdivision, int w, int h,
        SDL_Surface* outputSurface, int outputDivision){
    DEBUG("initializing matrix");
    Assert(outputSurface, "no output surface");
    this->outputSurface = outputSurface;
    this->outputDivision = outputDivision;

    int tileWidth = Cache::getTileWidth(subdivision);
    int tileHeight = Cache::getTileHeight(subdivision);

    Assert(w*tileWidth <= surface->w, "");
    Assert(h*tileHeight <= surface->h, "");

    width = w;
    height = h;
    this->subdivision = subdivision;

    matrix.resize(width);
    for (int x = 0; x < width; x++) {
        matrix[x].resize(h);

        for (int y = 0; y < height; y++) {
            matrix[x][y].stackData.loadFromSurface(surface,
                    x * tileWidth, y * tileHeight, subdivision);
        }
    }
}

ThumbnailMatrix::ThumbnailMatrix(){
  subdivisionFitCutoff.resize(ThumbnailStack::SIZE);
  subdivisionFitCutoffOriginal.resize(ThumbnailStack::SIZE);
}
