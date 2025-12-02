#include "SimpleCppLogger/Logger.h"

#include <memory>
#include <mutex>
#include <vector>

#include "SimpleCppLogger/ConsoleAppender.h"
#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/LogMessage.h"
#include "SimpleCppLogger/SimpleFormatter.h"

namespace SimpleCppLogger
{

/**
 * @class Logger::Impl
 * @brief Internal implementation for Logger (Pimpl idiom).
 */
class Logger::Impl
{
    public:
        Impl(): m_log_level(LogLevel::Debug)
        {
            // Add a default console appender with a simple formatter
            m_appenders.push_back(std::make_shared<ConsoleAppender>());
        }

        auto log(LogLevel level, const std::string& message,
                 const std::source_location& location) -> void
        {
            bool valid = (level >= LogLevel::Trace && level < LogLevel::Count);
            LogMessage log_message(level, message);

            if (valid && level >= m_log_level)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                for (const auto& appender: m_appenders)
                {
                    if (appender)
                    {
                        appender->append(log_message, location);
                    }
                }
            }
        }

        auto add_appender(const std::shared_ptr<LogAppender>& appender) -> void
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_appenders.push_back(appender);
        }

        auto clear_appenders() -> void
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_appenders.clear();
        }

        auto set_log_level(LogLevel level) -> void
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_log_level = level;
            for (const auto& appender: m_appenders)
            {
                if (appender)
                {
                    appender->set_log_level(level);
                }
            }
        }

        auto get_log_level() const -> LogLevel
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_log_level;
        }

    private:
        mutable std::mutex m_mutex;
        std::vector<std::shared_ptr<LogAppender>> m_appenders;
        LogLevel m_log_level;
};

Logger::Logger(): m_impl(std::make_unique<Impl>()) {}

Logger::~Logger() = default;

auto Logger::log(LogLevel level, const std::string& message,
                 const std::source_location& location) -> void
{
    m_impl->log(level, message, location);
}

auto Logger::log(LogLevel level, const std::string& message, const char* file, int line,
                 const char* function, const char* category) -> void
{
    std::string composed = message;

    const bool has_category = (category != nullptr && category[0] != '\0');
    const bool has_file = (file != nullptr && file[0] != '\0');
    const bool has_function = (function != nullptr && function[0] != '\0');
    const bool has_line = (line > 0);

    if (has_category || has_file || has_function || has_line)
    {
        composed += " - ";
        if (has_category)
        {
            composed += "[";
            composed += category;
            composed += "] ";
        }

        if (has_file)
        {
            composed += file;
            if (has_line)
            {
                composed += ":";
                composed += std::to_string(line);
            }
        }
        else if (has_line)
        {
            composed += ":";
            composed += std::to_string(line);
        }

        if (has_function)
        {
            if (has_file || has_line)
            {
                composed += ", ";
            }
            composed += function;
        }
    }

    log(level, composed, std::source_location{});
}

auto Logger::add_appender(const std::shared_ptr<LogAppender>& appender) -> void
{
    m_impl->add_appender(appender);
}

auto Logger::clear_appenders() -> void
{
    m_impl->clear_appenders();
}

auto Logger::set_log_level(LogLevel level) -> void
{
    m_impl->set_log_level(level);
}

auto Logger::get_log_level() const -> LogLevel
{
    return m_impl->get_log_level();
}

}  // namespace SimpleCppLogger
