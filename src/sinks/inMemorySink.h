#pragma once
#include <vector>

#include "level.h"
#include "pattern.h"
#include "sink.h"

namespace clogr
{
class InMemorySink final : public Sink
{
public:
    void handle(std::string_view msg, Level level, std::string_view loggerName, const Pattern& pattern) override;
    bool shouldLog(Level level) override;

    std::vector<std::string> getMessages();
private:
    std::vector<std::string> m_messages;
};
}
