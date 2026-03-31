#include "clogr.h"

#include "sinks/basicSink.h"

namespace clogr
{
#ifdef _WIN32
#include <windows.h>
    void initTerminal()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        SetConsoleOutputCP(CP_UTF8);
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, dwMode);
        }
    }
#else
    void initTerminal() {} // No-op for Linux/macOS
#endif

    bool initialized = false;
    grl::Rc<Logger> g_defaultLogger  = nullptr;

    grl::Rc<Logger> defaultLogger()
    {
        if(!initialized)
        {
            initTerminal();

            g_defaultLogger = grl::makeRc<Logger>("Logger");
            g_defaultLogger->addSink<BasicSink>();

            initialized = true;
        }

        return g_defaultLogger;
    }

    void setDefaultLogger(const grl::Rc<Logger> &logger)
    {
        g_defaultLogger = logger;
    }

    void trace(std::string_view msg)
    {
        defaultLogger()->log(Level::Trace, "{}", msg);
    }

    void info(std::string_view msg)
    {
        defaultLogger()->log(Level::Info, "{}", msg);
    }

    void warn(std::string_view msg)
    {
        defaultLogger()->log(Level::Warn, "{}", msg);
    }

    void error(std::string_view msg)
    {
        defaultLogger()->log(Level::Error, "{}", msg);
    }

    void fatal(std::string_view msg)
    {
        defaultLogger()->log(Level::Fatal, "{}", msg);
    }

    void flush()
    {
        defaultLogger()->flush();
    }

    namespace fs = std::filesystem;

    std::string formatStacktrace(const cpptrace::stacktrace &trace)
    {
        fmt::memory_buffer buf;

        // 1. Header: Thread Information
        auto thread_id = std::this_thread::get_id();
        fmt::format_to(std::back_inserter(buf),
            "{}\n",
            fmt::styled(fmt::format("--- Stack Trace (Thread: {}) ---", thread_id),
            fmt::fg(fmt::color::floral_white) | fmt::emphasis::underline)
        );

        for (size_t i = 0; i < trace.frames.size(); ++i)
        {
            const auto& frame = trace.frames[i];

            // 2. Filter Noise
            std::string sym = frame.symbol;
            if (sym.empty() ||
                sym.find("BaseThreadInitThunk") != std::string::npos ||
                sym.find("RtlUserThreadStart") != std::string::npos) {
                continue;
            }

            // 3. Frame & Function (Bold White/Cyan)
            fmt::format_to(std::back_inserter(buf),
                "{} {}\n",
                fmt::styled(fmt::format("{:2d}", i), fmt::fg(fmt::color::cyan)),
                fmt::styled(sym, fmt::emphasis::bold | fmt::fg(fmt::color::white))
            );

            // 4. Source Location
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
