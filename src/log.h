#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/callback_sink.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"
#include "streams/stream.h"

namespace Neon::Log
{
    void init(const std::string & logger_name);
    void setCustomPattern(const std::string & pattern);
    void setStreams(const std::vector<Stream*>& setStreams);
    void addStream(Stream* setStreams);

    template <typename ... Args>
    void info(const std::string& fmt, Args &&... args)
    {
        spdlog::info(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void warning(const std::string& fmt, Args &&... args)
    {
        spdlog::warn(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void error(const std::string& fmt, Args &&... args)
    {
        spdlog::error(fmt::runtime(fmt), std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void fatal(const std::string& fmt, Args &&... args)
    {
        spdlog::critical(fmt::runtime(fmt), std::forward<Args>(args)...);
    }
}
