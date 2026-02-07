#include "pattern.h"

#include <thread>
#include <chrono>
#include <fmt/std.h>

#include "fmt/args.h"

namespace clogr
{
    Pattern::Pattern(const std::string_view pattern)
    {
        m_patternStr = pattern;
    }

    std::string Pattern::format(const std::string_view message, const std::string_view loggerName, const Level level) const
    {
        thread_local fmt::dynamic_format_arg_store<fmt::format_context> store;

        store.clear();

        const auto now = std::chrono::system_clock::now();
        const auto ymd = std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(now) };
        const auto days = std::chrono::floor<std::chrono::days>(now); // The date
        const auto time_since_midnight = now - days;                  // The duration (time part)
        const auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_midnight);
        const std::chrono::hh_mm_ss tod{ time_since_midnight };

        store.push_back(fmt::arg("v", message));
        store.push_back(fmt::arg("n", loggerName));
        store.push_back(fmt::arg("l", level::toString(level)));
        store.push_back(fmt::arg("L", level::toShortString(level)));
        store.push_back(fmt::arg("t", std::this_thread::get_id()));

        // Date/Time
        store.push_back(fmt::arg("date", ymd));
        store.push_back(fmt::arg("time", fmt::format("{:%H:%M:%S}", time_ms)));
        store.push_back(fmt::arg("Y", std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(now)}));
        store.push_back(fmt::arg("m", ymd.month()));
        store.push_back(fmt::arg("d", ymd.day()));
        store.push_back(fmt::arg("H", tod.hours().count()));
        store.push_back(fmt::arg("M", tod.minutes().count()));
        store.push_back(fmt::arg("S", tod.seconds().count()));
        store.push_back(fmt::arg("e", tod.subseconds().count()));

        const auto us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        store.push_back(fmt::arg("f", fmt::format("{:05}", us.count())));
        // Color support
        store.push_back(fmt::arg("color", level::toColor(level)));
        store.push_back(fmt::arg("reset", "\033[0m"));

        return fmt::vformat(m_patternStr, store);
    }

    const std::string & Pattern::string() const
    {
        return m_patternStr;
    }
}
