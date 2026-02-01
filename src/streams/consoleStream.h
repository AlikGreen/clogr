#pragma once
#include "stream.h"

namespace Neon::Log
{
class ConsoleStream final : public Stream
{
public:
    explicit ConsoleStream(const Level minLogLevel = Level::Trace) : Stream(minLogLevel) {  }

    void handle(std::string formattedMsg, std::string rawMsg, Level level) override;
};
}
