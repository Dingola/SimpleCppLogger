#pragma once

#include <memory>
#include <source_location>
#include <string>

#include "ApiMacro.h"
#include "SimpleCppLogger/LogFormatter.h"
#include "SimpleCppLogger/LogMessage.h"

namespace SimpleCppLogger
{
/**
 * @class LogAppender
 * @brief An abstract base class for log appenders.
 *
 * This class defines the interface for log appenders.
 */
class SIMPLECPPLOGGER_API LogAppender
{
    public:
        LogAppender();
        explicit LogAppender(const std::shared_ptr<LogFormatter>& formatter);
        virtual ~LogAppender() = default;

        auto append(const LogMessage& message,
                    const std::source_location& location = std::source_location::current()) -> void;
        auto set_formatter(const std::shared_ptr<LogFormatter>& formatter) -> void;
        auto set_log_level(LogLevel level) -> void;
        [[nodiscard]] auto get_log_level() const -> LogLevel;

    protected:
        std::shared_ptr<LogFormatter> m_formatter;
        LogLevel m_log_level;

    private:
        /**
         * @brief Appends a log message to the log appender.
         *
         * This method is called by the public `append` method to actually append the specified log
         * message to the log appender. The log message is only appended if its type is greater than
         * or equal to the log level of the appender, which is checked in the `append` method.
         *
         * @param message The log message to append.
         * @param location The source location of the log message.
         */
        virtual auto internal_append(const LogMessage& message,
                                     const std::source_location& location) -> void = 0;
};
}  // namespace SimpleCppLogger
