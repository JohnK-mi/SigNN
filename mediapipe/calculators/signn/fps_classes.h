#include <time.h>
#include <queue>

#ifndef FPS_CLASSES_H
#define FPS_CLASSES_H

class FPSHistory{
    public:
        FPSHistory();
        FPSHistory(double, int);
        void push_back(clock_t);
        double average_fps();
    private:
        void remove_expired();
        std::deque<clock_t> raw_history_q;
        double MEMORY_IN_SECONDS;
        double delta_t(clock_t, clock_t);
        int MINIMUM_FRAMES_IN_MEMORY;
};

class FPS{
    public:
        FPS();
        FPS(double);
        double get_fps();
    private:
        double fps;
};

#endif