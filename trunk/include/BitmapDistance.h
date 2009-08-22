#ifndef H_BITMAP_DISTANCE
#define H_BITMAP_DISTANCE

#include "Bitmap.h"
#include "ColorAdjust.h"

class BitmapDistance {
  public:
    double distance(Bitmap* b1, Bitmap* b2, ColorAdjust* adjust);
};

#endif
