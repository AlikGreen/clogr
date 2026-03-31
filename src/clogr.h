#pragma once
#include <sstream>
#include <string_view>
#include <fmt/chrono.h>
#include <fmt/std.h>

#include "logger.h"
#include "cpptrace/cpptrace.hpp"

namespace clogr
{
    grl::Rc<Logger> defaultLogger();
    void setDefaultLogger(const grl::Rc<Logger>& logger);

    template <typename ... Args>
    void trace(fmt::format_string<Args...> fmt, Args &&... args)
    {
        defaultLogger()->log(Level::Trace, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void info(fmt::format_string<Args...> fmt, Args &&... args)
    {
        defaultLogger()->log(Level::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void warn(fmt::format_string<Args...> fmt, Args &&... args)
    {
        defaultLogger()->log(Level::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void error(fmt::format_string<Args...> fmt, Args &&... args)
    {
        defaultLogger()->log(Level::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void fatal(fmt::format_string<Args...> fmt, Args &&... args)
    {
        defaultLogger()->log(Level::Fatal, fmt, std::forward<Args>(args)...);
    }

    void trace(std::string_view msg);
    void info(std::string_view msg);
    void warn(std::string_view msg);
    void error(std::string_view msg);
    void fatal(std::string_view msg);

    void flush();

    std::string formatStacktrace(const cpptrace::stacktrace& trace);

    template<typename... Args>
    void ensure(const bool condition, fmt::format_string<Args...> fmt, Args&&... args)
    {
        if (condition)
            return;

        defaultLogger()->ensure(condition, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    [[noreturn]] void abort(fmt::format_string<Args...> fmtStr, Args&&... args)
    {
        const auto trace = cpptrace::generate_trace();
        fatal("[ABORTION] {}\n{}\n",
              fmt::format(fmtStr, std::forward<Args>(args)...),
              formatStacktrace(trace));
        std::exit(-1);
    }
}
