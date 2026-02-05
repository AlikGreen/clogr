#pragma once
#include "logger.h"
#include "sink.h"

namespace clogr
{
class BasicSink final : public Sink
{
public:
    explicit BasicSink(const Level minLogLevel = Level::Trace)
        : m_minLogLevel(minLogLevel) {  }


    void handle(std::string_view msg, Level level, std::string_view loggerName, const Pattern& pattern) override;
    bool shouldLog(Level level) override;
    void flush() override;
private:
    Level m_minLogLevel;
};
}
