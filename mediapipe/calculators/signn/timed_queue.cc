#include <vector>
#include <queue>
#include <chrono>


#include "mediapipe/calculators/signn/timed_queue.h"



// class TimedQueue{
//     public:
//     TimedQueue(double memory_in_miliseconds){
//         memory_length = memory_in_miliseconds;
//     }
//     void add(T adding){
//         dispenser.push_back(adding);
//         times.push(std::chrono::high_resolution_clock::now());
//     }
//     std::vector<T> get(){
//         auto front = times.front();
//         while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-front).count() > memory_length){
//             dispenser.erase(dispenser.begin());
//             times.pop();
//             front = times.front();
//         }
//         return dispenser;
//     }
//     private:
//         std::queue<std::chrono::_V2::system_clock::time_point> times;
//         std::vector<T> dispenser; 
//         double memory_length;
// };
template <class T>
TimedQueue<T>::TimedQueue(){
    memory_length = 1000;
}
template <class T>
TimedQueue<T>::TimedQueue(double memory_in_miliseconds){
    memory_length = memory_in_miliseconds;
}
template <class T>
void TimedQueue<T>::add(T adding){
    dispenser.push_back(adding);
    times.push(std::chrono::high_resolution_clock::now());
}
template <class T>
std::vector<T> TimedQueue<T>::get(){
    auto front = times.front();
    while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-front).count() > memory_length){
        dispenser.erase(dispenser.begin());
        times.pop();
        front = times.front();
    }
    return dispenser;
}

template class TimedQueue<int>;

// FPSHistory::FPSHistory(double const memory_in_seconds, int const minimum_frames_in_memory){
//     MEMORY_IN_SECONDS = memory_in_seconds;
//     MINIMUM_FRAMES_IN_MEMORY = minimum_frames_in_memory;
// }
// FPSHistory::FPSHistory(){

// }
// void FPSHistory::push_back(clock_t element){
//     raw_history_q.push_back(element);
//     remove_expired();
// }
// double FPSHistory::average_fps(){
//     double time_between_calls =  delta_t(raw_history_q.front(), raw_history_q.back()) / raw_history_q.size();
//     if(time_between_calls == 0){
//         return 0;
//     }
//     return 1 / time_between_calls;
// }
// void FPSHistory::remove_expired(){
//     if(raw_history_q.size() < MINIMUM_FRAMES_IN_MEMORY){
//         return;
//     }
//     if(delta_t(raw_history_q.front(), raw_history_q.back()) > MEMORY_IN_SECONDS){
//         raw_history_q.pop_front();
//     }
// }
// double FPSHistory::delta_t(clock_t start, clock_t end){
//     return (double)(end - start) / CLOCKS_PER_SEC;
// }

// FPS::FPS(double set_fps){
//     fps = set_fps;
// }
// double FPS::get_fps(){
//     return fps;
// }
