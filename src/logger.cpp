#include "logger.h"

#include <utility>

namespace clogr
{
    Logger::Logger(std::string name)
        : m_name(std::move(name))
    { }

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

    void Logger::setMinLevel(Level level)
    {
        m_minLogLevel = level;
    }
}
