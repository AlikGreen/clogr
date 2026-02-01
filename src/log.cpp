#include "log.h"

#include "callbackSink.h"
#include "streams/consoleStream.h"

namespace Neon::Log
{
    static auto streams = std::vector<Stream*>{};

    void init(const std::string &logger_name)
    {
        const auto logger = spdlog::stdout_color_mt(logger_name);
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%H:%M:%S] [thread %t] [%n] [%^%L%$] %v");

        streams.push_back(new ConsoleStream());

        const auto default_logger = spdlog::default_logger();
        auto callback_sink_ptr = std::make_shared<CallbackSink>([](const std::string& formatted, const std::string& raw, const Level level)
        {
            for (const auto& stream : streams)
            {
                if (levelToInt(level) >= levelToInt(stream->getMinLogLevel()))
                {
                    stream->handle(formatted, raw, level);
                }
            }
        });
        default_logger->sinks() = {callback_sink_ptr};
    }

    void setCustomPattern(const std::string &pattern)
    {
        spdlog::set_pattern(pattern);
    }

    void setStreams(const std::vector<Stream*>& setStreams)
    {
        streams = setStreams;
    }

    void addStream(Stream *setStreams)
    {
        streams.push_back(setStreams);
    }
}
