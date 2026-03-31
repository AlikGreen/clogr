#pragma once

#include "logger.h"
#include "sink.h"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <utility>

namespace clogr
{
class FileSink final : public Sink
{
public:
    explicit FileSink(std::filesystem::path filePath,
        const Level minLogLevel = Level::Trace,
        const std::uintmax_t maxFileSize = 10 * 1024 * 1024,
        const unsigned maxFiles = 5)
        : m_filePath(std::move(filePath))
        , m_minLogLevel(minLogLevel)
        , m_maxFileSize(maxFileSize)
        , m_maxFiles(maxFiles)
    {
        openStream();
    }

    ~FileSink() override
    {
        std::lock_guard lock(m_mutex);
        if (m_ofstream.is_open())
        {
            m_ofstream.flush();
            m_ofstream.close();
        }
    }

    void handle(std::string_view msg, Level level, std::string_view loggerName, const Pattern& pattern) override;
    bool shouldLog(Level level) override;
    void flush() override;

private:
    void rotateIfNeeded();
    void rotate();
    void openStream();
    [[nodiscard]] std::uintmax_t currentFileSize() const;

    std::filesystem::path m_filePath;
    Level m_minLogLevel;
    std::uintmax_t m_maxFileSize;
    unsigned m_maxFiles;

    std::ofstream m_ofstream;
    std::mutex m_mutex;
};
}