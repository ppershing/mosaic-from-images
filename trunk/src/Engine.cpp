#include "Engine.h"
#include "SDL.h"

#include "Debug.h"
#include "MyStringUtils.h"

void Engine::initialize() {
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
            case SDL_VIDEORESIZE:
             int w = event.resize.w;
             int h = event.resize.h;
            
              SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE|SDL_RESIZABLE);
              break;
            default:
                Errors::_addError("unhandled event", Errors::WARNING);
        }
    }
}

void Engine::doWork(){
    project->fitNextImage();
    timer.tick();
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
        doWork();
        saveWork();
        if (tick % 2 == 0) {
            printf("FPS: %f\n", timer.getFPS());
        }
    }
}
