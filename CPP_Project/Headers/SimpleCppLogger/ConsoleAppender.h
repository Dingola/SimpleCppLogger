#pragma once

#include "ApiMacro.h"
#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/SimpleFormatter.h"

namespace SimpleCppLogger
{
/**
 * @class ConsoleAppender
 * @brief A log appender that outputs log messages to the console.
 *
 * This class is responsible for appending log messages to the console.
 * It uses a provided LogFormatter to format the log messages before outputting them.
 */
class SIMPLECPPLOGGER_API ConsoleAppender: public LogAppender
{
    public:
        /**
         * @brief Constructs a ConsoleAppender object.
         *
         * This constructor initializes the ConsoleAppender object with the provided LogFormatter.
         * If no formatter is provided, a default SimpleFormatter is used.
         *
         * @param formatter The LogFormatter object to use for formatting log messages.
         */
        explicit ConsoleAppender(
            const std::shared_ptr<LogFormatter>& formatter = std::make_shared<SimpleFormatter>());

    private:
        /**
         * @brief Appends the specified log message to the console.
         *
         * This method formats the log message using the provided formatter and outputs it to the
         * console using std::cout or std::cerr depending on the log level.
         *
         * @param message The log message to append to the console.
         */
        auto internal_append(const LogMessage& message,
                             const std::source_location& location) -> void override;
};
}  // namespace SimpleCppLogger
