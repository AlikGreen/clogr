#pragma once
#include <string>

namespace clogr
{
class Sink
{
public:
    Sink() = default;
    virtual ~Sink() = default;

    Sink(const Sink&) = delete;
    Sink& operator=(const Sink&) = delete;


    virtual void handle(std::string_view msg, Level level, std::string_view loggerName, const Pattern& pattern) = 0;
    virtual bool shouldLog(Level level) = 0;
    virtual void flush() {};
};
}
