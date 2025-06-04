#include "SimpleCppLogger/ConsoleAppender.h"

#include <iostream>

namespace SimpleCppLogger
{

/**
 * @brief Constructs a ConsoleAppender object.
 *
 * This constructor initializes the ConsoleAppender object with the provided LogFormatter.
 * If no formatter is provided, a default SimpleFormatter is used.
 *
 * @param formatter The LogFormatter object to use for formatting log messages.
 */
ConsoleAppender::ConsoleAppender(const std::shared_ptr<LogFormatter>& formatter)
    : LogAppender(formatter)
{}

/**
 * @brief Appends the specified log message to the console.
 *
 * This method formats the log message using the provided formatter and outputs it to the
 * console using std::cout or std::cerr depending on the log level.
 *
 * @param message The log message to append to the console.
 * @param location The source location of the log message.
 */
auto ConsoleAppender::internal_append(const LogMessage& message,
                                      const std::source_location& location) -> void
{
    const std::string formatted_message = m_formatter->format(message, location);

    switch (message.get_level())
    {
    case LogLevel::Info:
    case LogLevel::Debug:
        std::cout << formatted_message << std::endl;
        break;
    case LogLevel::Warning:
    case LogLevel::Error:
    case LogLevel::Fatal:
        std::cerr << formatted_message << std::endl;
        break;
    default:
        std::cout << formatted_message << std::endl;
        break;
    }
}

}  // namespace SimpleCppLogger
