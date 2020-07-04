#include <chrono>
#include <iostream>
#include <time.h>
#include <queue>

#include "mediapipe/calculators/signn/fps_classes.h"


FPSHistory::FPSHistory(double const memory_in_seconds, int const minimum_frames_in_memory){
    MEMORY_IN_SECONDS = memory_in_seconds;
    MINIMUM_FRAMES_IN_MEMORY = minimum_frames_in_memory;
}
FPSHistory::FPSHistory(){

}
void FPSHistory::push_back(clock_t element){
    raw_history_q.push_back(element);
    remove_expired();
}
double FPSHistory::average_fps(){
    double time_between_calls =  delta_t(raw_history_q.front(), raw_history_q.back()) / raw_history_q.size();
    if(time_between_calls == 0){
        return 0;
    }
    return 1 / time_between_calls;
}
void FPSHistory::remove_expired(){
    if(raw_history_q.size() < MINIMUM_FRAMES_IN_MEMORY){
        return;
    }
    if(delta_t(raw_history_q.front(), raw_history_q.back()) > MEMORY_IN_SECONDS){
        raw_history_q.pop_front();
    }
}
double FPSHistory::delta_t(clock_t start, clock_t end){
    return (double)(end - start) / CLOCKS_PER_SEC;
}

FPS::FPS(double set_fps){
    fps = set_fps;
}
double FPS::get_fps(){
    return fps;
}