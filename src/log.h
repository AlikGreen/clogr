#pragma once
#include "callbackSink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/callback_sink.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"

namespace Neon::Log
{
    inline void init(const std::string & logger_name)
    {
        const auto logger = spdlog::stdout_color_mt(logger_name);
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%H:%M:%S] [thread %t] [%n] [%^%L%$] %v");
    }

    inline void setCustomPattern(const std::string & pattern)
    {
        spdlog::set_pattern(pattern);
    }

    inline void setCustomCallback(const std::function<void(const std::string&, Level)>& callback)
    {
        const auto default_logger = spdlog::default_logger();
        auto callback_sink_ptr = std::make_shared<CallbackSink>(callback);
        default_logger->sinks() = {callback_sink_ptr};
    }

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
