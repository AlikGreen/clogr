#pragma once
#include "spdlog/spdlog.h"

namespace Neon
{
    enum Level : int
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
            case spdlog::level::trace: return Trace; break;
            case spdlog::level::debug: return None; break;
            case spdlog::level::info: return Info; break;
            case spdlog::level::warn: return Warn; break;
            case spdlog::level::err: return Error; break;
            case spdlog::level::critical: return Fatal; break;
            default: return None;
        }
    }

}
