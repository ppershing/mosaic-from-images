// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "Engine.h"
#include "SDL.h"

#include "Debug.h"
#include "MyStringUtils.h"
#include <stdlib.h>
#include <time.h>

void Engine::initialize() {
    DEBUG("initializing random number generator");
    srand(time(NULL));
    DEBUG("load global preferences");

    preferences.loadFromFile("Mosaic.ini");

    DEBUG("loading cache");
    cache.loadFromFile(preferences.getString("/cache/data"));
    DEBUG("cache loaded");

    std::string projectDir = preferences.getString("/project/dir");
    std::string projectFile = preferences.getString("/project/file");
    project=new Project(&cache);

    project->load(projectDir, projectFile);

    timer.start();
    saveTime = preferences.getDouble("/save/time");
}

void Engine::finalize(){
    project->save();
    project->saveOutput();
    delete project;
}

void Engine::handleEvents(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym==SDLK_ESCAPE) run = 0;
                if (event.key.keysym.sym==SDLK_s) {
                    project->save();
                    project->saveOutput();
                    project->showStatistics();
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                project->mouseClick(event.button.x, event.button.y);
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_VIDEOEXPOSE:
                break;
            case SDL_SYSWMEVENT:
                break;
            case SDL_VIDEORESIZE:
            
              SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_SWSURFACE|SDL_RESIZABLE);
              break;
            default:
                Errors::_addError("unhandled event", Errors::WARNING);
        }
    }
}

int Engine::doWork(){
    int ret = project->fitNextImage();
    timer.tick();
    return ret;
}

void Engine::saveWork(){
    if (timer.getTimeFromStart() > lastSaveTime + saveTime) {
        project->save();
        project->saveOutput();
        project->showStatistics();
        lastSaveTime = timer.getTimeFromStart();
    }
}

void Engine::runMainLoop(){
    run = 1;
    tick = 0;

    while(run){
        tick++;
        handleEvents();
        int didWork = doWork();
        if (!didWork) run=0;
        saveWork();
        if (tick % 30 == 0) {
            double fps=timer.getFPS();
            printf("FPS: %f, ETA %.1f hours\n", fps,
                    project->numRemainingImages()/fps/3600.0);
        }
    }
}
