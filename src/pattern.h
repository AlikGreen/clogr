#pragma once
#include <string>
#include <string_view>

#include "level.h"

namespace clogr
{
class Pattern
{
public:
    explicit Pattern(std::string_view pattern);

    [[nodiscard]] std::string format(std::string_view message, std::string_view loggerName, Level level) const;
    [[nodiscard]] const std::string& string() const;
private:
    std::string m_patternStr;
};
}
