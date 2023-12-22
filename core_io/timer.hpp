#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    Timer()
    {
        start = std::chrono::steady_clock::now();
    }
    std::time_t get_time_ms()
    {
        auto duration = get_time_stamp();
        return duration.count();
    }
    friend std::ostream& operator<<(std::ostream& os,const Timer & t)
    {
        auto duration = t.get_time_stamp();
        // 计算小时、分钟、秒和毫秒
        long long hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        long long minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1)).count();
        long long seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1)).count();
        long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration % std::chrono::seconds(1)).count();

        // 格式化为"00:00:00.000"的形式
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;

        os << ss.str();
        return os;
    }
private:
    std::chrono::steady_clock::time_point start;
    std::chrono::milliseconds get_time_stamp() const
    {
        auto now_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - start);
        return duration;
    }
}
;
#endif