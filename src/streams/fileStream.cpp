#include "fileStream.h"

#include <filesystem>
#include <bits/fs_path.h>
#include <bits/random.h>

namespace Neon::Log
{
    FileStream::FileStream(const std::string& logDirectory, const std::string& filePrefix, Level minLogLevel): Stream(minLogLevel)
    {
        namespace fs = std::filesystem;
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6);

        const std::string fileName = filePrefix + "_" + std::to_string(dist6(rng)) + ".log";
        outfile = std::ofstream(fs::path(logDirectory) / fs::path(fileName));
    }

    void FileStream::handle(const std::string msg)
    {
        outfile << msg << "\n";

        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        if(duration > 100)
        {
            startTime = std::chrono::high_resolution_clock::now();
            outfile.flush();
        }
    }

    void FileStream::close()
    {
        outfile.close();
    }
}
