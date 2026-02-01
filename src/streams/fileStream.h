#pragma once
#include <fstream>

#include "stream.h"

namespace Neon::Log
{
class FileStream final : public Stream
{
public:
    explicit FileStream(const std::string& logDirectory, const std::string& filePrefix, Level minLogLevel = Level::Trace);
    void handle(std::string formattedMsg, std::string rawMsg, Level level) override;
    void close() override;
private:
    std::ofstream outfile;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};
}
