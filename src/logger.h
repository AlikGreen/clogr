#pragma once
#include <level.h>
#include <string_view>
#include <utility>
#include <vector>
#include <grl/grl.h>

#include "pattern.h"
#include "cpptrace/cpptrace.hpp"
#include "fmt/format.h"
#include "sinks/sink.h"

namespace clogr
{
enum class AssertBehavior
{
    Abort, Throw, Ignore
};

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
    void setEnsureBehaviour(AssertBehavior behavior);

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

    std::string formatStacktrace(const cpptrace::stacktrace& trace);

    template<typename... Args>
    void ensure(const bool condition, fmt::format_string<Args...> fmt, Args&&... args)
    {
        if(condition) return;

        const auto trace = cpptrace::generate_trace();

        switch (m_assertBehaviour)
        {
            case AssertBehavior::Abort:
                this->log(Level::Fatal, "[ASSERTION] {}\n{}\n", fmt::format(fmt, std::forward<Args>(args)...), formatStacktrace(trace));
                std::exit(-1);
            case AssertBehavior::Throw:
                throw std::runtime_error(fmt::format("Assertion failed: {}\n\n{}", fmt::format(fmt, std::forward<Args>(args)...), formatStacktrace(trace)));
            default:
                break;
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
    AssertBehavior m_assertBehaviour = AssertBehavior::Abort;
};
}
