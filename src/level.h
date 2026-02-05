#pragma once
#include <string>

#include "fmt/color.h"

namespace clogr
{
    enum class Level : int
    {
        Trace,
        Info,
        Warn,
        Error,
        Fatal,
        None
    };

    namespace level
    {
        static int toInt(const Level level)
        {
            switch (level)
            {
                case Level::Trace: return 0;
                case Level::Info:  return 1;
                case Level::Warn:  return 2;
                case Level::Error: return 3;
                case Level::Fatal: return 4;
                case Level::None:  return 5;
                default: return 0;
            }
        }

        static std::string toString(const Level level)
        {
            switch (level)
            {
                case Level::Trace: return "Trace";
                case Level::Info:  return "Info";
                case Level::Warn:  return "Warn";
                case Level::Error: return "Error";
                case Level::Fatal: return "Fatal";
                case Level::None:  return "None";
                default: return "This shouldn't happen";
            }
        }

        static std::string toShortString(const Level level)
        {
            switch (level)
            {
                case Level::Trace: return "T";
                case Level::Info:  return "I";
                case Level::Warn:  return "W";
                case Level::Error: return "E";
                case Level::Fatal: return "F";
                case Level::None:  return "N";
                default: return "This shouldn't happen";
            }
        }

        static std::string toColor(const Level level)
        {
            switch (level) {
                case Level::Trace:    return "\033[37m";      // White/Gray
                case Level::Info:     return "\033[34m";      // Green
                case Level::Warn:     return "\033[33m";      // Yellow
                case Level::Error:    return "\033[31m";      // Red
                case Level::Fatal:    return "\033[31;1m";    // Bold Red
                case Level::None:     return "";
                default:              return "\033[0m";       // Reset
            }
        }
    }
}