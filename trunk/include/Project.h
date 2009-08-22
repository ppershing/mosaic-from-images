#ifndef H_PROJECT
#define H_PROJECT

#include "Cache.h"
#include "SDL.h"
#include <string>
#include "Preferences.h"
#include "ThumbnailMatrix.h"
#include "ProgressStatistics.h"
#include "VariableOptimizer.h"

class BitmapFitOptimizerFunction: public VariableOptimizerFunction {
 public:
     void setBitmaps(Bitmap* b1, Bitmap* b2);
     virtual double evaluate(std::vector<double> value);
 private:
    Bitmap* b1;
    Bitmap* b2;
    ColorAdjust adjust;
    BitmapDistance distance;
};


class Project{
  public:
    Project(Cache* cache);
    ~Project();
    void load(const std::string& dir,
              const std::string& file);
    void save();
    void saveOutput();
    void showStatistics();

    void fitNextImage();
  private:
    void loadImageStack();
    void loadMapping();
    void createOutputSurface();

    void clean();
    Cache* cache;
    SDL_Surface* outputSurface;
    Preferences preferences;
    Preferences skipImages;
    Preferences thumbnailMapping;
    int tilesX;
    int tilesY;
    int tileDivision;
    int outputDivision;

    std::string inputImageFile;
    std::string outputImageFile;
    std::string skipFile;
    std::string mappingFile;
    std::string projectFile;
    std::vector<std::string> cacheEnumeration;
    int cacheEnumerationId;
    ThumbnailMatrix thumbnailMatrix;
    ProgressStatistics progressStatistics;
    VariableOptimizer optimizer;
    BitmapFitOptimizerFunction optimizerFunction;
};


#endif
