#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include "spdlog/spdlog.h"
#include <cpptrace/cpptrace.hpp>

namespace Neon::Debug
{
    inline std::string cleanFunctionName(const std::string& symbol)
    {
        std::string cleaned = symbol;

        // Remove std::__cxx11 noise (I think this is good)
        size_t pos;
        while ((pos = cleaned.find("std::__cxx11::")) != std::string::npos)
            cleaned.erase(pos, 14);

        // maybe shouldn't do this
        while ((pos = cleaned.find(", std::allocator<")) != std::string::npos)
        {
            size_t end = cleaned.find('>', pos);
            if (end != std::string::npos)
                cleaned.erase(pos, end - pos + 1);
        }


        // maybe shouldn't remove these
        while ((pos = cleaned.find(" const&")) != std::string::npos)
            cleaned.erase(pos, 7);

        while ((pos = cleaned.find(" const")) != std::string::npos)
            cleaned.erase(pos, 6);

        return cleaned;
    }

    inline std::string formatStackTrace(const cpptrace::stacktrace& trace)
    {
        std::ostringstream oss;

        int frameNum = 0;
        for (size_t i = 0; i < trace.frames.size(); ++i)
        {
            const auto& frame = trace.frames[i];

            // Skip internal/useless frames
            if (frame.filename.empty() && frame.symbol.empty())
                continue;

            if (frame.symbol.find("__tmainCRTStartup") != std::string::npos ||
                frame.symbol.find("BaseThreadInitThunk") != std::string::npos ||
                frame.symbol.find("RtlUserThreadStart") != std::string::npos ||
                frame.symbol.find("__invoke") != std::string::npos ||
                frame.symbol.find("_M_invoke") != std::string::npos)
                continue;

            // Frame number
            oss << "  [" << frameNum++ << "] ";

            // Function name
            if (!frame.symbol.empty())
            {
                oss << cleanFunctionName(frame.symbol) << "\n";
            }
            else
            {
                oss << "??\n";
            }

            // File location - KEEP COMPLETELY INTACT ON ONE LINE
            if (!frame.filename.empty() && frame.line.has_value())
            {
                oss << "      at " << frame.filename << ":" << frame.line.value() << "\n";
            }

            oss << "\n";
        }

        return oss.str();
    }

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

        spdlog::critical("============================================================");
        spdlog::critical("ASSERTION FAILED");
        spdlog::critical("Message: {}", fmt::format(fmtStr, std::forward<Args>(args)...));
        spdlog::critical("============================================================");

        auto trace = cpptrace::generate_trace();
        spdlog::critical("\nCall Stack:\n{}", formatStackTrace(trace));

        std::exit(-1);
    }
}