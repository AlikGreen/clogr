#include "clog.h"

namespace clogr
{
    Neon::Rc<Logger> defaultLogger = Neon::makeRc<Logger>("Logger");


    Neon::Rc<Logger> getDefaultLogger()
    {
        return defaultLogger;
    }

    void setDefaultLogger(const Neon::Rc<Logger> &logger)
    {
        defaultLogger = logger;
    }

    void trace(std::string_view msg)
    {
        getDefaultLogger()->log(Level::Trace, "{}", msg);
    }

    void info(std::string_view msg)
    {
        getDefaultLogger()->log(Level::Info, "{}", msg);
    }

    void warn(std::string_view msg)
    {
        getDefaultLogger()->log(Level::Warn, "{}", msg);
    }

    void error(std::string_view msg)
    {
        getDefaultLogger()->log(Level::Error, "{}", msg);
    }

    void fatal(std::string_view msg)
    {
        getDefaultLogger()->log(Level::Fatal, "{}", msg);
    }

    void flush()
    {
        getDefaultLogger()->flush();
    }

    std::string formatStacktrace(const cpptrace::stacktrace &trace)
    {
        fmt::memory_buffer buf;

        for (size_t i = 0; i < trace.frames.size(); ++i)
        {
            const auto& frame = trace.frames[i];

            // Skip garbage frames early
            if (frame.is_inline) continue; // optional: skip inlined frames

            // Skip known noise (you can customize this list)
            if (frame.symbol.find("RtlUserThreadStart") != std::string::npos ||
                frame.symbol.find("BaseThreadInitThunk") != std::string::npos ||
                frame.symbol.find("__scrt_common_main_seh") != std::string::npos ||
                frame.symbol.empty() && frame.filename.empty())
                continue;

            // Frame number (bold cyan)
            fmt::format_to(std::back_inserter(buf),
                " {}", // Space before
                fmt::styled(fmt::format("[{}]", i), fmt::emphasis::bold | fmt::fg(fmt::color::cyan))
            );

            // Symbol — this is the magic: cpptrace already demangles!
            std::string symbol = frame.symbol.empty() ? "???" : frame.symbol;

            // Optional: truncate very long symbols (common with deep templates)
            if (symbol.size() > 120) {
                symbol = symbol.substr(0, 117) + "...";
            }

            fmt::format_to(std::back_inserter(buf), FMT_STRING("{}\n"), symbol);

            // Source location (dimmed)
            if (!frame.filename.empty() && frame.line.has_value())
            {
                fmt::format_to(std::back_inserter(buf),
                    "     {} {}:{}\n",
                    fmt::styled("at", fmt::fg(fmt::color::dark_gray)), // Style applied here
                    frame.filename,
                    frame.line.value()
                );
            }

            fmt::format_to(std::back_inserter(buf), "\n");
        }

        return fmt::to_string(buf);
    }
}
