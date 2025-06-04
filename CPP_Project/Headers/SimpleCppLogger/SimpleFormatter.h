#pragma once

#include <source_location>
#include <string>

#include "ApiMacro.h"
#include "SimpleCppLogger/LogFormatter.h"

namespace SimpleCppLogger
{
/**
 * @class SimpleFormatter
 * @brief A simple log formatter that formats log messages with type, date, time, and context.
 *
 * This class provides a simple implementation of the LogFormatter interface.
 * It formats log messages by including the message type, current date and time,
 * the message itself, and the file, line, and function where the log was generated.
 */
class SIMPLECPPLOGGER_API SimpleFormatter: public LogFormatter
{
    public:
        /**
         * @brief Constructs a SimpleFormatter object.
         */
        SimpleFormatter() = default;

        /**
         * @brief Formats the log message according to the specified context.
         *
         * @param log_message The log message to format.
         * @param location The source location of the log message.
         * @return The formatted log message as a std::string.
         */
        [[nodiscard]] auto format(
            const LogMessage& log_message,
            const std::source_location& location = std::source_location::current()) const
            -> std::string override;
};

}  // namespace SimpleCppLogger
