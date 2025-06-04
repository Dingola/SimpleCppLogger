#include "SimpleCppLogger/LogMessage.h"

namespace SimpleCppLogger
{
/**
 * @brief Constructs a LogMessage object with the given log level and message.
 * @param level The log level of the message.
 * @param message The content of the log message.
 */
LogMessage::LogMessage(LogLevel level, std::string message)
    : m_level(level), m_message(std::move(message))
{}

/**
 * @brief Gets the log level of the log message.
 * @return The log level of the log message.
 */
auto LogMessage::get_level() const -> LogLevel
{
    return m_level;
}

/**
 * @brief Gets the content of the log message.
 * @return The content of the log message.
 */
auto LogMessage::get_message() const -> const std::string&
{
    return m_message;
}
}  // namespace SimpleCppLogger
