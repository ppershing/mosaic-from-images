// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#ifndef H_THUMBNAIL_MATRIX
#define H_THUMBNAIL_MATRIX

#include "ThumbnailStack.h"
#include "ThumbnailFitData.h"
#include "Preferences.h"
#include "BitmapDistance.h"
#include "ProgressStatistics.h"
#include <vector>
#include <map>
#include "ColorAdjust.h"

class ThumbnailMatrixData {
    public:
    ThumbnailStack stackData;
    ThumbnailFitData fitData;
};

class ThumbnailMatrix{
  public:
    void initialize(Preferences* mapping, Preferences* skip, int
            sameImageMax);
    void initializeMatrix(SDL_Surface* surface, int subdivision,
               int tilesX, int tilesY,
               SDL_Surface* outputSurface, int outputDivision);

    double getImageFit(ThumbnailStack* stack, ColorAdjust* adjust,
                int subdiv, int x, int y);

    void placeImageAt(std::string& hash, ThumbnailStack* stack,
            ColorAdjust* adjust, int x, int y);

    bool canPlaceThumbnail(std::string hash);


    std::string getCellSummary(int x,int y);

    std::string getMappingString(int x,int y);

    ThumbnailMatrixData& getData(int x,int y);

    void saveFitData(const std::string& filename);


    double getSubdivisionFitCutoff(int subdivision);
    double getSubdivisionFitCutoffOriginal(int subdivision);
    void recomputeSubdivisionFitCutoffs();

    ThumbnailMatrix();

  private:
    std::vector<double> subdivisionFitCutoff;
    std::vector<double> subdivisionFitCutoffOriginal;

    std::vector<std::vector<ThumbnailMatrixData> > matrix;
    Preferences* mapping;
    Preferences* skip;

    int width;
    int height;
    int subdivision;
    std::map<std::string, int> imageCount;
    int sameImageMax;
    BitmapDistance distanceFunction;
    SDL_Surface* outputSurface;
    int outputDivision;

};

#endif
