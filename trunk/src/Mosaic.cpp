/*
    Mosaic maker,
    (c) ppershing, ppershing@fks.sk, 2009
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "Errors.h"
#include "Debug.h"
#include "Version.h"
#include "Engine.h"

Engine engine;

void init(){
    Errors::createInstance("Mosaic.log");

    DEBUG("init sdl");
    /* Initialize the SDL library */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        throw Exception("Couldn't initialize SDL");
    }

    SDL_SetVideoMode(200,200,32,SDL_RESIZABLE|SDL_SWSURFACE);

    engine.initialize();
}

void finalize(){
    engine.finalize();
    /* We're done! */
    SDL_Quit();
    Errors::destroyInstance();
}

void run(){
    Errors::_addError("running main loop", Errors::NOTICE);
    engine.runMainLoop();
}


int main(int argc, char *argv[])
{
    printf("%s\n", Version::getFullProductName().c_str());
    try {
        init();
        Errors::_addError("init done",Errors::NOTICE);
        run();

        Errors::_addError("finalizing",Errors::NOTICE);
        finalize();

    } catch (Exception& e){

        fprintf(stderr,"There was an exception :\n%s\n ", e.what());

    } catch (std::exception& e){
        fprintf(stderr,"There was an exception :\n%s\n",e.what());
    }

    return(0);
}
