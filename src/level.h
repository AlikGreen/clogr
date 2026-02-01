#pragma once
#include "spdlog/spdlog.h"

namespace Neon
{
    enum class Level : int
    {
        Trace,
        Info,
        Warn,
        Error,
        Fatal,
        None
    };

    static Level spdToLevel(const spdlog::level::level_enum level)
    {
        switch (level)
        {
            case spdlog::level::trace: return Level::Trace; break;
            case spdlog::level::debug: return Level::None; break;
            case spdlog::level::info: return Level::Info; break;
            case spdlog::level::warn: return Level::Warn; break;
            case spdlog::level::err: return Level::Error; break;
            case spdlog::level::critical: return Level::Fatal; break;
            default: return Level::None;
        }
    }

    static int levelToInt(const Level level)
    {
        switch (level)
        {
            case Level::Trace: return 0; break;
            case Level::Info: return 1; break;
            case Level::Warn: return 2; break;
            case Level::Error: return 3; break;
            case Level::Fatal: return 4; break;
            case Level::None: return 5; break;
            default: return 0;
        }
    }

}
