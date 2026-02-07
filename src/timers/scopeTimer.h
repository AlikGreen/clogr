#pragma once
#include <chrono>

#include <clogr.h>

namespace clogr
{
    template <typename DurationType = std::chrono::microseconds>
    class ScopeTimer
    {
    public:
        explicit ScopeTimer(const char* name = "Scope") : name(name), start(std::chrono::high_resolution_clock::now()) {  }
        explicit ScopeTimer(const std::function<void(int64_t)> &callback) : callback(callback), start(std::chrono::high_resolution_clock::now()) {  }

        ~ScopeTimer()
        {
            const auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<DurationType>(end - start);
            if(callback != nullptr)
                callback(duration.count());
            else
                clogr::info("[{}] duration: {} {}", name, duration.count(), get_unit_name<DurationType>());

        }

    private:
        const char* name = nullptr;
        std::function<void(int64_t)> callback = nullptr;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;

        template <typename T>
        static constexpr const char* get_unit_name()
        {
            if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) return "nanoseconds";
            else if constexpr (std::is_same_v<T, std::chrono::microseconds>) return "microseconds";
            else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) return "milliseconds";
            else if constexpr (std::is_same_v<T, std::chrono::seconds>) return "seconds";
            else if constexpr (std::is_same_v<T, std::chrono::minutes>) return "minutes";
            else if constexpr (std::is_same_v<T, std::chrono::hours>) return "hours";
            else return "units";
        }
    };
}
