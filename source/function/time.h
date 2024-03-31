#pragma once
#include <chrono>
#include <unistd.h>
#include <string>
#include <map>
using namespace std::chrono;
using namespace std;
#include <iostream>
namespace WuXing
{
    class TIME {
    private:
        static inline steady_clock::time_point m_last_time;
        static inline float sys_time = 0.f;
        static inline float delta_time = 0.f;
    public:
        static void init() { m_last_time = steady_clock::now(); }
        
        static void tick() {
            steady_clock::time_point current_time = steady_clock::now();
            duration<float> time_span = duration_cast<duration<float>>(current_time - m_last_time);
            delta_time = time_span.count();
            m_last_time = current_time;
            sys_time += delta_time;
        }
        static float deltaTime()    { return delta_time; }
        static float sysTime()      { return sys_time; }
    };
} // namespace WuXing
