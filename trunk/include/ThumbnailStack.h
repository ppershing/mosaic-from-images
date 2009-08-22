#ifndef H_THUMBNAIL_STACK
#define H_THUMBNAIL_STACK

#include "CacheData.h"
#include "Bitmap.h"
#include <vector>

class ThumbnailStack {
  public:
      ThumbnailStack();
      static const int SIZE=7;
      std::vector<Bitmap> stack;
      void loadFromCache(CacheData& data, int minDivision);
      void loadFromSurface(SDL_Surface* surface, int x, int y, int
              initialDivision);

  private:
      void loadFromSurfaceInternal(SDL_Surface* surface, int
              initialDivision);


};

#endif
