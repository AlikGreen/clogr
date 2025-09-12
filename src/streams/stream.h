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

    virtual void handle(std::string msg) = 0;
    virtual void close() {  };
private:
    Level minLogLevel = Trace;
};
}
