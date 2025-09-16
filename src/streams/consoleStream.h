#pragma once
#include "stream.h"

namespace Neon::Log
{
class ConsoleStream final : public Stream
{
public:
    explicit ConsoleStream(const Level minLogLevel = Trace) : Stream(minLogLevel) {  }

    void handle(std::string msg) override;
};
}
