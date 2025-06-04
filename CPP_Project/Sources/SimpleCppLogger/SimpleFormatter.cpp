/**
 * @file SimpleFormatter.cpp
 * @brief This file contains the implementation of the SimpleFormatter class.
 */

#include "SimpleCppLogger/SimpleFormatter.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "SimpleCppLogger/LogLevel.h"

namespace SimpleCppLogger
{

namespace
{
inline std::string get_current_datetime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#if defined(_WIN32)
    localtime_s(&tm_buf, &now_c);
#else
    localtime_r(&now_c, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
}  // namespace

auto SimpleFormatter::format(const LogMessage& log_message,
                             const std::source_location& location) const -> std::string
{
    std::string msg_type;
    std::string color_code;

    switch (log_message.get_level())
    {
    case LogLevel::Debug:
        msg_type = "[Debug    ]:";
        color_code = "\033[92m";  // Light Green
        break;
    case LogLevel::Warning:
        msg_type = "[Warning  ]:";
        color_code = "\033[93m";  // Light Yellow
        break;
    case LogLevel::Info:
        msg_type = "[Info     ]:";
        color_code = "\033[94m";  // Light Blue
        break;
    case LogLevel::Error:
        msg_type = "[Error    ]:";
        color_code = "\033[91m";  // Light Red
        break;
    case LogLevel::Fatal:
        msg_type = "[Fatal    ]:";
        color_code = "\033[95m";  // Light Magenta
        break;
    case LogLevel::Trace:
        msg_type = "[Trace    ]:";
        color_code = "\033[96m";  // Light Cyan
        break;
    default:
        msg_type = "[Unknown  ]:";
        color_code = "\033[0m";  // Reset
        break;
    }

    const std::string reset_code = "\033[0m";
    const std::string context_color_code = "\033[95m";  // Light Purple

    std::ostringstream oss;
    oss << color_code << msg_type << reset_code << " " << get_current_datetime() << " "
        << log_message.get_message() << " - " << context_color_code << location.file_name()
        << reset_code << ":" << context_color_code << location.line() << reset_code << ", "
        << context_color_code << location.function_name() << reset_code;

    return oss.str();
}

}  // namespace SimpleCppLogger
