#pragma once
#include <chrono>

namespace Neon
{
class Timer
{
public:
    void begin()
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void end()
    {
        endTime = std::chrono::high_resolution_clock::now();
    }

    template <typename DurationType = std::chrono::microseconds>
    [[nodiscard]] int64_t getElapsedTime() const
    {
        return std::chrono::duration_cast<DurationType>(endTime - startTime);
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
};
}
