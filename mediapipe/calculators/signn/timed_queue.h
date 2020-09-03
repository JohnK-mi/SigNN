#include <chrono>
#include <vector>
#include <queue>

#ifndef TIMED_QUEUE_H
#define TIMED_QUEUE_H

template <class T>
class TimedQueue{
    public:
        TimedQueue(double);
        TimedQueue();
        void add(T);
        std::vector<T> get();
    private:
        std::queue<std::chrono::_V2::system_clock::time_point> times;
        std::vector<T> dispenser; 
        double memory_length;
        
};

#endif