#include "SimpleCppLogger/LogAppender.h"

#include "SimpleCppLogger/SimpleFormatter.h"

namespace SimpleCppLogger
{

/**
 * @brief Constructs a LogAppender object with a default SimpleFormatter.
 *
 * This constructor initializes the LogAppender object with a default SimpleFormatter
 * and sets the log level to LogLevel::Info.
 */
LogAppender::LogAppender()
    : m_formatter(std::make_shared<SimpleFormatter>()), m_log_level(LogLevel::Info)
{}

/**
 * @brief Constructs a LogAppender object.
 *
 * This constructor initializes the LogAppender object with the provided LogFormatter
 * and sets the log level to LogLevel::Info.
 *
 * @param formatter The LogFormatter object to use for formatting log messages.
 */
LogAppender::LogAppender(const std::shared_ptr<LogFormatter>& formatter)
    : m_formatter(formatter), m_log_level(LogLevel::Debug)
{}

/**
 * @brief Appends a log message to the log appender.
 *
 * This method checks if the log message level is greater than or equal to the log level
 * of the appender. If it is, the message is appended by calling the internal_append method.
 *
 * @param message The log message to append.
 * @param location The source location of the log message.
 */
auto LogAppender::append(const LogMessage& message, const std::source_location& location) -> void
{
    if (message.get_level() >= m_log_level)
    {
        internal_append(message, location);
    }
}

/**
 * @brief Sets the formatter for the log appender.
 *
 * This method sets the formatter used by the log appender to format log messages.
 *
 * @param formatter The formatter to set.
 */
auto LogAppender::set_formatter(const std::shared_ptr<LogFormatter>& formatter) -> void
{
    m_formatter = formatter;
}

/**
 * @brief Sets the log level of the log appender.
 *
 * This method sets the log level of the log appender. Messages with a level lower than
 * this level will be discarded.
 *
 * @param level The log level to set.
 */
auto LogAppender::set_log_level(LogLevel level) -> void
{
    m_log_level = level;
}

/**
 * @brief Returns the current log level of the log appender.
 *
 * This method returns the current log level of the log appender.
 *
 * @return The current log level of the log appender.
 */
auto LogAppender::get_log_level() const -> LogLevel
{
    return m_log_level;
}

}  // namespace SimpleCppLogger
