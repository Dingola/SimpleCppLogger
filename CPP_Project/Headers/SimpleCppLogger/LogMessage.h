#pragma once

#include <string>

#include "ApiMacro.h"
#include "SimpleCppLogger/LogLevel.h"

namespace SimpleCppLogger
{
class SIMPLECPPLOGGER_API LogMessage
{
    public:
        LogMessage(LogLevel level = LogLevel::Info, std::string message = std::string());
        virtual ~LogMessage() = default;

        [[nodiscard]] auto get_level() const -> LogLevel;
        [[nodiscard]] auto get_message() const -> const std::string&;

    private:
        LogLevel m_level;
        std::string m_message;
};
}  // namespace SimpleCppLogger
