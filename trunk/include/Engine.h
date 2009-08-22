#ifndef H_ENGINE
#define H_ENGINE
#include "Project.h"
#include "Preferences.h"
#include "Cache.h"
#include "Timer.h"

class Engine{
    public:
        void initialize();
        void finalize();
        void runMainLoop();

    private:
        void handleEvents();
        void doWork();
        void saveWork();
        Project* project;
        Preferences preferences;
        Cache cache;

        int run;
        int tick;
        Timer timer;
        double lastSaveTime;
        double saveTime;
};

#endif
