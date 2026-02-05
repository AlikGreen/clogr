#include "basicSink.h"

#include <iostream>

namespace clogr
{
    void BasicSink::handle(const std::string_view msg, const Level level, const std::string_view loggerName, const Pattern& pattern)
    {
        std::cout << pattern.format(msg, loggerName, level) << "\n";
    }

    bool BasicSink::shouldLog(const Level level)
    {
        if(level::toInt(level) >= level::toInt(m_minLogLevel))
            return true;

        return false;
    }

    void BasicSink::flush()
    {
        std::cout << std::flush;
    }
}
