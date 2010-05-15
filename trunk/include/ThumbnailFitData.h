// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#ifndef H_THUMBNAIL_FIT_DATA
#define H_THUMBNAIL_FIT_DATA

#include <string>
#include <vector>
#include "ColorAdjust.h"


class ThumbnailFitData{
  public:
      ThumbnailFitData();
    std::string thumbnailHash;
    ColorAdjust adjust;
    double diff;
    std::vector<double> diffStack;
    std::vector<double> diffStackOriginal;
    std::string getFitStatistics();
};

#endif
