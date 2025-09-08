#pragma once
#include <string>

#include "spdlog/spdlog.h"

namespace Neon::Debug
{
    template <typename ... Args>
    void ensure(const bool condition, const std::string& fmt, Args &&... args)
    {
        if (condition) return;

        spdlog::critical(fmt::runtime("{}"+fmt), "ASSERTION FAILED:\n",std::forward<Args>(args)...);
        std::abort();
    }

}
