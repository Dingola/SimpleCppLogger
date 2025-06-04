#pragma once

#include <cstddef>
#include <optional>
#include <string_view>

#include "ApiMacro.h"

/**
 * @file LogLevel.h
 * @brief Defines the LogLevel enumeration and provides string conversion utilities.
 */

namespace SimpleCppLogger
{

/**
 * @def LOGLEVEL_LIST
 * @brief Macro list for LogLevel enum and string mapping.
 *
 * Each entry consists of:
 *   - Enum value
 *   - Narrow string literal
 *   - Wide string literal
 */
#define LOGLEVEL_LIST                 \
    X(Trace, "Trace", L"Trace")       \
    X(Debug, "Debug", L"Debug")       \
    X(Info, "Info", L"Info")          \
    X(Warning, "Warning", L"Warning") \
    X(Error, "Error", L"Error")       \
    X(Fatal, "Fatal", L"Fatal")

/**
 * @enum LogLevel
 * @brief Represents the severity level of a log message.
 *
 * The order of the values defines their severity for comparison.
 */
enum class SIMPLECPPLOGGER_API LogLevel
{
#define X(a, b, c) a,
    LOGLEVEL_LIST
#undef X
        Count
};

/**
 * @brief Array of narrow string representations for each LogLevel.
 */
constexpr inline const char* LogLevelNamesA[] = {
#define X(a, b, c) b,
    LOGLEVEL_LIST
#undef X
};

/**
 * @brief Array of wide string representations for each LogLevel.
 */
constexpr inline const wchar_t* LogLevelNamesW[] = {
#define X(a, b, c) c,
    LOGLEVEL_LIST
#undef X
};

/**
 * @brief Array of std::string_view representations for each LogLevel.
 */
constexpr inline std::string_view LogLevelNamesSv[] = {
#define X(a, b, c) b,
    LOGLEVEL_LIST
#undef X
};

/**
 * @brief Array of std::wstring_view representations for each LogLevel.
 */
constexpr inline std::wstring_view LogLevelNamesWsv[] = {
#define X(a, b, c) c,
    LOGLEVEL_LIST
#undef X
};

/**
 * @brief The number of log levels defined.
 */
constexpr inline std::size_t LogLevelCount = static_cast<std::size_t>(LogLevel::Count);

/**
 * @brief Converts a LogLevel to its narrow string representation.
 * @param level The log level.
 * @return The corresponding const char* string, or an empty string if out of range.
 */
[[nodiscard]] constexpr inline auto to_string(LogLevel level) -> const char*
{
    auto index = static_cast<std::size_t>(level);
    return (index >= LogLevelCount) ? "" : LogLevelNamesA[index];
}

/**
 * @brief Converts a LogLevel to its wide string representation.
 * @param level The log level.
 * @return The corresponding const wchar_t* string, or an empty string if out of range.
 */
[[nodiscard]] constexpr inline auto to_wstring(LogLevel level) -> const wchar_t*
{
    auto index = static_cast<std::size_t>(level);
    return (index >= LogLevelCount) ? L"" : LogLevelNamesW[index];
}

/**
 * @brief Converts a LogLevel to its std::string_view representation.
 * @param level The log level.
 * @return The corresponding std::string_view, or an empty view if out of range.
 */
[[nodiscard]] constexpr inline auto to_string_view(LogLevel level) -> std::string_view
{
    auto index = static_cast<std::size_t>(level);
    return (index >= LogLevelCount) ? std::string_view{} : LogLevelNamesSv[index];
}

/**
 * @brief Converts a LogLevel to its std::wstring_view representation.
 * @param level The log level.
 * @return The corresponding std::wstring_view, or an empty view if out of range.
 */
[[nodiscard]] constexpr inline auto to_wstring_view(LogLevel level) -> std::wstring_view
{
    auto index = static_cast<std::size_t>(level);
    return (index >= LogLevelCount) ? std::wstring_view{} : LogLevelNamesWsv[index];
}

/**
 * @brief Converts a std::string_view to a LogLevel.
 * @param sv The string view to parse.
 * @return The corresponding LogLevel, or std::nullopt if not found.
 */
[[nodiscard]] constexpr inline auto from_string_view(std::string_view sv) -> std::optional<LogLevel>
{
    std::size_t found = LogLevelCount;

    for (std::size_t i = 0; i < LogLevelCount; ++i)
    {
        if (LogLevelNamesSv[i] == sv)
        {
            found = i;
        }
    }

    return (found < LogLevelCount) ? std::optional<LogLevel>{static_cast<LogLevel>(found)}
                                   : std::nullopt;
}

/**
 * @brief Converts a std::wstring_view to a LogLevel.
 * @param sv The wide string view to parse.
 * @return The corresponding LogLevel, or std::nullopt if not found.
 */
[[nodiscard]] constexpr inline auto from_wstring_view(std::wstring_view sv)
    -> std::optional<LogLevel>
{
    std::size_t found = LogLevelCount;

    for (std::size_t i = 0; i < LogLevelCount; ++i)
    {
        if (LogLevelNamesWsv[i] == sv)
        {
            found = i;
        }
    }

    return (found < LogLevelCount) ? std::optional<LogLevel>{static_cast<LogLevel>(found)}
                                   : std::nullopt;
}

}  // namespace SimpleCppLogger
