#pragma once

#include <CommonLib/Patterns/Singleton.h>

#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include <vector>

#include "ApiMacro.h"
#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/LogLevel.h"

namespace SimpleCppLogger
{
/**
 * @class Logger
 * @brief A singleton logger for logging messages with various severity levels.
 *
 * This class provides methods to log messages, manage log appenders, and set the log level.
 * It supports thread-safe operations and allows multiple appenders for flexible output.
 */
class SIMPLECPPLOGGER_API Logger: public CommonLib::Singleton<Logger>
{
        friend class CommonLib::Singleton<Logger>;

    private:
        Logger();
        ~Logger() override;

        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        /**
         * @brief Logs a message with the specified log level and source location.
         * @param level The severity level of the log message.
         * @param message The log message content.
         * @param location The source location of the log message (defaults to caller location).
         */
        auto log(LogLevel level, const std::string& message,
                 const std::source_location& location = std::source_location::current()) -> void;

        /**
         * @brief Adds a log appender to the logger.
         * @param appender The log appender to add.
         */
        auto add_appender(const std::shared_ptr<LogAppender>& appender) -> void;

        /**
         * @brief Removes all log appenders from the logger.
         */
        auto clear_appenders() -> void;

        /**
         * @brief Sets the log level of the logger.
         * @param level The log level to set.
         */
        auto set_log_level(LogLevel level) -> void;

        /**
         * @brief Returns the current log level of the logger.
         * @return The current log level.
         */
        [[nodiscard]] auto get_log_level() const -> LogLevel;
};

}  // namespace SimpleCppLogger
