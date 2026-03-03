#include "logger.h"

#include <thread>
#include <utility>
#include <filesystem>

namespace clogr
{
    Logger::Logger(std::string name)
        : m_name(std::move(name)),
    m_pattern("[{time}]{color}[{l}]{reset} {v}")
    {

    }

    void Logger::setPattern(const std::string_view pattern)
    {
        m_pattern = Pattern(pattern);
    }

    void Logger::flush()
    {
        for(const auto& sink : m_sinks)
        {
            sink->flush();
        }
    }

    void Logger::clearSinks()
    {
        m_sinks.clear();
    }

    void Logger::setMinLevel(const Level level)
    {
        m_minLogLevel = level;
    }

    void Logger::setEnsureBehaviour(const AssertBehavior behavior)
    {
        m_assertBehaviour = behavior;
    }

    std::string Logger::formatStacktrace(const cpptrace::stacktrace &trace)
    {
        namespace fs = std::filesystem;
        fmt::memory_buffer buf;

        // Header: Thread Information
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        fmt::format_to(std::back_inserter(buf),
            "{}\n",
            fmt::styled(fmt::format("--- Stack Trace (Thread: {}) ---", oss.str()),
            fmt::fg(fmt::color::floral_white) | fmt::emphasis::underline)
        );

        for (size_t i = 0; i < trace.frames.size(); ++i)
        {
            const auto& frame = trace.frames[i];

            // Filter Noise
            std::string sym = frame.symbol;
            if (sym.empty() ||
                sym.find("BaseThreadInitThunk") != std::string::npos ||
                sym.find("RtlUserThreadStart") != std::string::npos) {
                continue;
            }

            // Frame & Function (Bold White/Cyan)
            fmt::format_to(std::back_inserter(buf),
                "{} {}\n",
                fmt::styled(fmt::format("{:2d}", i), fmt::fg(fmt::color::cyan)),
                fmt::styled(sym, fmt::emphasis::bold | fmt::fg(fmt::color::white))
            );

            // Source Location
            if (!frame.filename.empty())
            {
                // Unify slashes to \ for Windows or / for Linux
                fs::path p = fs::path(frame.filename).make_preferred();
                std::string path_str = p.string();

                // Shorten path: Look for your project folder
                size_t pos = path_str.find("IonEngine");
                if (pos != std::string::npos) {
                    path_str = ".." + path_str.substr(pos - 1);
                }

                // High-visibility colors: Yellow for file, Green for line
                fmt::format_to(std::back_inserter(buf), "   {} ",
                    fmt::styled("└─", fmt::fg(fmt::color::gray)));

                fmt::format_to(std::back_inserter(buf), "  {}:{}\n",
                    p.string(), // Use the full path from fs::path
                    frame.line.value_or(0)
                );
            }
        }

        return fmt::to_string(buf);
    }
}
