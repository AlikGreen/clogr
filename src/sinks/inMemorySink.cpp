#include "inMemorySink.h"

namespace clogr
{
    void InMemorySink::handle(const std::string_view msg, const Level level, const std::string_view loggerName, const Pattern &pattern)
    {
        m_messages.push_back(pattern.format(msg, loggerName, level));
    }

    bool InMemorySink::shouldLog(Level level)
    {
        return true;
    }

    std::vector<std::string> InMemorySink::getMessages()
    {
        return m_messages;
    }
}
