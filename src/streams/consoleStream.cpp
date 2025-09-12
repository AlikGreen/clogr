#include "consoleStream.h"

#include <iostream>

namespace Neon
{
    void Log::ConsoleStream::handle(const std::string msg)
    {
        std::cout << msg << "\n";
    }
}
