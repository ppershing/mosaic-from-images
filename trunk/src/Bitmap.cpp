// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "Bitmap.h"
#include "MyAssert.h"

void Bitmap::loadFromSurface(SDL_Surface* surface,
        int x, int y, int w, int h){
    SDL_PixelFormat *fmt=surface->format;

    Assert(surface->w >= x + w, "Bitmap::loadFromSurface");
    Assert(surface->h >= y + h, "Bitmap::loadFromSurface");
    Assert(surface->format->BitsPerPixel == 32,
            "Bitmap::blitToSurface wrong bits per pixel");

    this->width = w;
    this->height = h;

    r.resize(w);
    g.resize(w);
    b.resize(w);

    for (int i=0; i<w; i++) {
        r[i].resize(h);
        g[i].resize(h);
        b[i].resize(h);

        for (int j=0; j<h; j++) {
            Uint32 pixel = *((Uint32*) surface->pixels +
                      ((j+y)*surface->w + i+x));
            r[i][j] = (((pixel & fmt->Rmask) >> fmt->Rshift) 
                        << fmt->Rloss) / 255.0;
            g[i][j] = (((pixel & fmt->Gmask) >> fmt->Gshift) 
                        << fmt->Gloss) / 255.0;
            b[i][j] = (((pixel & fmt->Bmask) >> fmt->Bshift) 
                        << fmt->Bloss) / 255.0;
        }
    }
}

void Bitmap::blitToSurface(SDL_Surface* surface, ColorAdjust* adjust,
        int x, int y) {
    SDL_PixelFormat* fmt = surface->format;
    Assert(surface->w >= x + width, "Bitmap::blitToSurface");
    Assert(surface->h >= y + height, "Bitmap::blitToSurface");
    Assert(surface->format->BitsPerPixel == 32,
            "Bitmap::blitToSurface wrong bits per pixel");

    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
            double cr = r[i][j]; 
            double cg = g[i][j];
            double cb = b[i][j];
            adjust->adjust(cr,cg,cb);

            Uint32 pixel=0;
            pixel |= Uint32(Uint8(cr * 255.0) >> fmt->Rloss)
                        << fmt->Rshift;
            pixel |= Uint32(Uint8(cg * 255.0) >> fmt->Gloss)
                        << fmt->Gshift;
            pixel |= Uint32(Uint8(cb * 255.0) >> fmt->Bloss)
                        << fmt->Bshift;

            Uint32* ptr = (Uint32*)surface->pixels + ((j+y)*surface->w + i + x);
            *(ptr) = pixel;
        }
    }
}

int Bitmap::getWidth(){
    return width;
}

int Bitmap::getHeight(){
    return height;
}
