#include "ThumbnailStack.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <string>
#include "MyAssert.h"
#include "Cache.h"

ThumbnailStack::ThumbnailStack(){
  stack.resize(SIZE);
}

void ThumbnailStack::loadFromCache(CacheData& data, int minDivision){
    std::string filename = data.getThumbnailFile();
    SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
    Assert(tmpSurface, "problem with loading thumbnail "+filename);
    SDL_Surface* surface = shrinkSurface(tmpSurface, 1<<minDivision,
             1<<minDivision);
    Assert(surface, "problem converting surface");
    SDL_FreeSurface(tmpSurface);

    loadFromSurfaceInternal(surface, minDivision);
    SDL_FreeSurface(surface);
}

void ThumbnailStack::loadFromSurface(SDL_Surface* surface, int x,int y, int
    initialDivision) {
    SDL_Surface* tmpSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
            Cache::getTileWidth(initialDivision),
            Cache::getTileHeight(initialDivision),
            32, 0,0,0,0);
    Assert(tmpSurface, "");

    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=Cache::getTileWidth(initialDivision);
    rect.h=Cache::getTileHeight(initialDivision);
    SDL_BlitSurface(surface, &rect, tmpSurface, NULL);

    loadFromSurfaceInternal(tmpSurface, initialDivision);

    SDL_FreeSurface(tmpSurface);
}

void ThumbnailStack::loadFromSurfaceInternal(SDL_Surface* surface, int
        initialDivision) {
    if (initialDivision >= SIZE) return;
    int w=Cache::getTileWidth(initialDivision);
    int h=Cache::getTileHeight(initialDivision);

    Assert(surface->w == w, "");
    Assert(surface->h == h, "");

    stack[initialDivision].loadFromSurface(surface, 0, 0, w, h);

    SDL_Surface* tmpSurface=shrinkSurface(surface, 2, 2);
    Assert(tmpSurface, "");

    loadFromSurfaceInternal(tmpSurface, initialDivision+1);
    SDL_FreeSurface(tmpSurface);
}
