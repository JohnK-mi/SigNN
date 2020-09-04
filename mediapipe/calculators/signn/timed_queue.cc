#include <vector>
#include <queue>
#include <chrono>


#include "mediapipe/calculators/signn/timed_queue.h"

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

template class TimedQueue<bool>;
template class TimedQueue<double>;