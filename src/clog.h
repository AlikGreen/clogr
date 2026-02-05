#pragma once
#include <sstream>
#include <string_view>
#include <fmt/chrono.h>
#include <fmt/std.h>

#include "logger.h"
#include "cpptrace/cpptrace.hpp"

namespace clogr
{
    Neon::Rc<Logger> getDefaultLogger();
    void setDefaultLogger(const Neon::Rc<Logger>& logger);

    template <typename ... Args>
    void trace(fmt::format_string<Args...> fmt, Args &&... args)
    {
        getDefaultLogger()->log(Level::Trace, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void info(fmt::format_string<Args...> fmt, Args &&... args)
    {
        getDefaultLogger()->log(Level::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void warn(fmt::format_string<Args...> fmt, Args &&... args)
    {
        getDefaultLogger()->log(Level::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void error(fmt::format_string<Args...> fmt, Args &&... args)
    {
        getDefaultLogger()->log(Level::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void fatal(fmt::format_string<Args...> fmt, Args &&... args)
    {
        getDefaultLogger()->log(Level::Fatal, fmt, std::forward<Args>(args)...);
    }

    void trace(std::string_view msg);
    void info(std::string_view msg);
    void warn(std::string_view msg);
    void error(std::string_view msg);
    void fatal(std::string_view msg);

    void flush();

    std::string formatStacktrace(const cpptrace::stacktrace& trace);

    template<typename... Args>
    void ensure
    (
        const bool condition,
        fmt::format_string<Args...> fmtStr,
        Args&&... args
    )
    {
        if (condition)
        {
            return;
        }

        fatal("============================================================");
        fatal("ASSERTION FAILED");
        fatal("Message: {}", fmt::format(fmtStr, std::forward<Args>(args)...));
        fatal("============================================================");

        auto trace = cpptrace::generate_trace();
        fatal("\nCall Stack:\n{}", formatStacktrace(trace));

        std::exit(-1);
    }
}
