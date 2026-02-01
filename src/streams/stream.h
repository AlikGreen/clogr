#pragma once
#include <string>

#include "level.h"

namespace Neon::Log
{
class Stream
{
public:
    virtual ~Stream() = default;
    explicit Stream(const Level minLogLevel) : minLogLevel(minLogLevel) { }

    virtual void handle(std::string formattedMsg, std::string rawMsg, Level level) = 0;
    virtual void close() {  };

    [[nodiscard]] Level getMinLogLevel() const
    {
        return minLogLevel;
    }
private:
    Level minLogLevel = Level::Trace;
};
}
