#pragma once
#include <level.h>
#include <string_view>
#include <utility>
#include <vector>
#include <grl/grl.h>

#include "pattern.h"
#include "fmt/format.h"
#include "sinks/sink.h"

namespace clogr
{

class Logger
{
public:
    explicit Logger(std::string name);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) noexcept = default;
    Logger& operator=(Logger&&) noexcept = default;

    void setPattern(std::string_view pattern);
    void flush();
    void clearSinks();
    void setMinLevel(Level level);

    template<typename... Args>
    void log(const Level level, fmt::format_string<Args...> fmt, Args&&... args)
    {
        if(level < m_minLogLevel) return;

        for(const auto& sink : m_sinks)
        {
            if(!sink->shouldLog(level)) continue;

            const std::string formatted = fmt::format(fmt, std::forward<Args>(args)...);
            sink->handle(formatted, level, m_name, m_pattern);
        }
    }

    template<typename T, typename... Args>
    requires (std::is_base_of_v<Sink, T> && std::is_constructible_v<T, Args...>)
    T& addSink(Args&&... args)
    {
        T* sink = new T(std::forward<Args>(args)...);
        m_sinks.push_back(grl::Box<T>(sink));
        return *sink;
    }
private:
    std::string m_name;
    std::vector<grl::Box<Sink>> m_sinks{};
    Pattern m_pattern;
    Level m_minLogLevel = Level::Trace;
};
}
