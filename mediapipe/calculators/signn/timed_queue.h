#include <time.h>
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
        std::queue<clock_t> times;
        std::vector<T> dispenser; 
        double memory_length;
        double delta_t(clock_t start, clock_t end);
        
};

#endif