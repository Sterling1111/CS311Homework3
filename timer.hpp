#ifndef TIMER_TIMER_HPP
#define TIMER_TIMER_HPP
#include<chrono>
#include <iostream>
#include <typeinfo>

typedef std::chrono::milliseconds milli;
typedef std::chrono::nanoseconds nano;
typedef std::chrono::microseconds micro;
typedef std::chrono::seconds seconds;

template<typename T = milli>
class Timer {
public:
    Timer();
    void Stop();
    ~Timer();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
};

template<typename T>
Timer<T>::Timer() {
    startTimePoint = std::chrono::high_resolution_clock::now();
}

template<typename T>
void Timer<T>::Stop() {
    auto endTimePoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<T>(startTimePoint).time_since_epoch().count();

    auto end = std::chrono::time_point_cast<T>(endTimePoint).time_since_epoch().count();
    auto total_time = end - start;

    if(typeid(T) == typeid(nano))
        std::cout << total_time << " ns\n";
    else if(typeid(T) == typeid(micro))
        std::cout << total_time << " us\n";
    else if(typeid(T) == typeid(milli))
        std::cout << total_time << " ms\n";
    else if(typeid(T) == typeid(seconds))
        std::cout << total_time << " s\n";
    else
        std::cout << total_time << std::endl;
}

template<typename T>
Timer<T>::~Timer() {
    Stop();
}

#endif //TIMER_TIMER_HPP
