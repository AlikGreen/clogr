#include "fileSink.h"

namespace clogr
{
    void FileSink::handle(const std::string_view msg, const Level level, const std::string_view loggerName, const Pattern& pattern)
    {
        const std::string output = pattern.format(msg, loggerName, level);

        if (m_ofstream.is_open())
        {
            m_ofstream << output << '\n';
            if (m_ofstream.fail())
            {
                // fallback to stderr if file write fails
                fmt::println(stderr, "{}", output);
            }
        }

        rotateIfNeeded();
    }

    bool FileSink::shouldLog(const Level level)
    {
        return level::toInt(level) >= level::toInt(m_minLogLevel);
    }

    void FileSink::flush()
    {
        std::lock_guard lock(m_mutex);
        if (m_ofstream.is_open())
            m_ofstream.flush();
    }

    void FileSink::openStream()
    {
        std::error_code ec;
        create_directories(m_filePath.parent_path(), ec);
        m_ofstream.open(m_filePath, std::ios::app | std::ios::binary);
    }

    std::uintmax_t FileSink::currentFileSize() const
    {
        std::error_code ec;
        const auto size = file_size(m_filePath, ec);
        if (ec)
            return 0;
        return size;
    }

    void FileSink::rotateIfNeeded()
    {
        try
        {
            if (m_maxFileSize == 0)
                return;

            const auto size = currentFileSize();
            if (size <= m_maxFileSize)
                return;

            rotate();
        }
        catch (...) {}
    }

    void FileSink::rotate()
    {
        if (m_ofstream.is_open())
            m_ofstream.close();

        std::error_code ec;

        for (int i = static_cast<int>(m_maxFiles) - 1; i >= 1; --i)
        {
            const auto from = m_filePath.string() + "." + std::to_string(i);
            const auto to = m_filePath.string() + "." + std::to_string(i + 1);

            const std::filesystem::path fromPath(from);
            const std::filesystem::path toPath(to);

            if (exists(toPath, ec))
                std::filesystem::remove(toPath, ec);

            if (exists(fromPath, ec))
                std::filesystem::rename(fromPath, toPath, ec);
        }

        const auto firstRotated = m_filePath.string() + ".1";
        const std::filesystem::path firstRotatedPath(firstRotated);

        if (exists(firstRotatedPath, ec))
            std::filesystem::remove(firstRotatedPath, ec);

        if (exists(m_filePath, ec))
            std::filesystem::rename(m_filePath, firstRotatedPath, ec);

        openStream();
    }
}