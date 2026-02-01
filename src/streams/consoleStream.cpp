#include "consoleStream.h"

#include <iostream>

namespace Neon
{
    void Log::ConsoleStream::handle(const std::string formattedMsg, const std::string rawMsg, Level level)
    {
        std::cout << formattedMsg << "\n";
    }
}
