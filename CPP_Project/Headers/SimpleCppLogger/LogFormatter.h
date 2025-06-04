#pragma once

#include <source_location>
#include <string>

#include "ApiMacro.h"
#include "SimpleCppLogger/LogMessage.h"

namespace SimpleCppLogger
{
/**
 * @class LogFormatter
 * @brief An abstract base class for log formatters.
 *
 * This class defines the interface for log formatters.
 */
class SIMPLECPPLOGGER_API LogFormatter
{
    public:
        /**
         * @brief Constructs a LogFormatter object.
         */
        LogFormatter() = default;

        /**
         * @brief Destroys the LogFormatter object.
         */
        virtual ~LogFormatter() = default;

        /**
         * @brief Formats the specified log message.
         *
         * @param log_message The log message to format.
         * @param location The source location of the log message.
         * @return The formatted log message as a std::string.
         */
        [[nodiscard]] virtual auto format(
            const LogMessage& log_message,
            const std::source_location& location = std::source_location::current()) const
            -> std::string = 0;
};

}  // namespace SimpleCppLogger
