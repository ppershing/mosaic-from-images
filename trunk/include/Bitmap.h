// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#ifndef H_BITMAP
#define H_BITMAP
#include "SDL/SDL.h"
#include <vector>
#include "ColorAdjust.h"

class Bitmap{
  public:
      int getWidth();
      int getHeight();

      inline float& getR(int x,int y){
        return r[x][y];
      }

      inline float& getG(int x,int y){
        return g[x][y];
      }

      inline float& getB(int x,int y){
        return b[x][y];
      }

    void loadFromSurface(SDL_Surface* surface, int x,int y,int w,int h);
    void blitToSurface(SDL_Surface* surface, ColorAdjust* adjust, int x,int y);

  private:
    std::vector<std::vector<float> > r,g,b;
    int width,height;

};

#endif
