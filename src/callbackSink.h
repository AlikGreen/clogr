#pragma once
#include "level.h"
#include "spdlog/sinks/base_sink.h"

namespace Neon::Log
{
    class CallbackSink final : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        explicit CallbackSink(std::function<void(const std::string&, const std::string&, Level)> callback) : callback(std::move(callback)) {}
    private:
        std::function<void(const std::string&, const std::string&, Level)> callback;
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            if (callback)
            {
                spdlog::memory_buf_t formatted;
                formatter_->format(msg, formatted);
                const std::string formattedStr = to_string(formatted);
                const std::string rawMessage(msg.payload.data(), msg.payload.size());
                callback(formattedStr, rawMessage, spdToLevel(msg.level));
            }
        }

        void flush_() override { }
    };
}
