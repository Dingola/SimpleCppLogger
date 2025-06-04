#include "SimpleCppLogger/SimpleFormatterTest.h"

#include <regex>
#include <source_location>
#include <string>

#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/LogMessage.h"
#include "SimpleCppLogger/SimpleFormatter.h"

using namespace SimpleCppLogger;

/**
 * @brief Tests that the formatted output contains the log level, message, and context.
 */
TEST_F(SimpleFormatterTest, ContainsLevelMessageAndContext)
{
    SimpleFormatter formatter;
    LogMessage msg(LogLevel::Warning, "Warnung aufgetreten");
    auto formatted = formatter.format(msg, std::source_location::current());

    EXPECT_NE(formatted.find("[Warning  ]:"), std::string::npos);
    EXPECT_NE(formatted.find("Warnung aufgetreten"), std::string::npos);
    EXPECT_NE(formatted.find(__FILE__), std::string::npos);
    EXPECT_NE(formatted.find(__func__), std::string::npos);
}

/**
 * @brief Tests that the formatted output contains the current date and time in the expected format.
 */
TEST_F(SimpleFormatterTest, ContainsCurrentDateTime)
{
    SimpleFormatter formatter;
    LogMessage msg(LogLevel::Info, "Zeitstempel-Test");
    auto formatted = formatter.format(msg, std::source_location::current());

    // Regex for "YYYY-MM-DD HH:MM:SS"
    std::regex datetime_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    EXPECT_TRUE(std::regex_search(formatted, datetime_regex));
}

/**
 * @brief Tests that all log levels are formatted with the correct label and color code.
 */
TEST_F(SimpleFormatterTest, AllLogLevelsFormatCorrectly)
{
    SimpleFormatter formatter;
    const char* expected_labels[] = {"[Trace    ]:", "[Debug    ]:", "[Info     ]:",
                                     "[Warning  ]:", "[Error    ]:", "[Fatal    ]:"};
    LogLevel levels[] = {LogLevel::Trace,   LogLevel::Debug, LogLevel::Info,
                         LogLevel::Warning, LogLevel::Error, LogLevel::Fatal};

    for (size_t i = 0; i < sizeof(levels) / sizeof(levels[0]); ++i)
    {
        LogMessage msg(levels[i], "Test");
        auto formatted = formatter.format(msg, std::source_location::current());
        EXPECT_NE(formatted.find(expected_labels[i]), std::string::npos);
    }
}

/**
 * @brief Tests that a long message is included in the formatted output.
 */
TEST_F(SimpleFormatterTest, LongMessageIncluded)
{
    SimpleFormatter formatter;
    std::string long_msg(10000, 'x');
    LogMessage msg(LogLevel::Info, long_msg);
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find(long_msg), std::string::npos);
}

/**
 * @brief Tests that a UTF-8 encoded message is included in the formatted output.
 */
TEST_F(SimpleFormatterTest, Utf8MessageIncluded)
{
    SimpleFormatter formatter;
    std::string utf8_msg = "Grüße 🌍";
    LogMessage msg(LogLevel::Debug, utf8_msg);
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find(utf8_msg), std::string::npos);
}

/**
 * @brief Tests that the formatted output contains the correct file, line, and function information.
 */
TEST_F(SimpleFormatterTest, ContainsFileLineFunction)
{
    SimpleFormatter formatter;
    LogMessage msg(LogLevel::Error, "Kontext-Test");
    auto location = std::source_location::current();
    auto formatted = formatter.format(msg, location);

    EXPECT_NE(formatted.find(location.file_name()), std::string::npos);
    EXPECT_NE(formatted.find(std::to_string(location.line())), std::string::npos);
    EXPECT_NE(formatted.find(location.function_name()), std::string::npos);
}

/**
 * @brief Tests that an unknown or out-of-range LogLevel is formatted as [Unknown  ]: and uses the reset color code.
 */
TEST_F(SimpleFormatterTest, UnknownLogLevelFormatsAsUnknown)
{
    SimpleFormatter formatter;
    LogLevel invalid_level = static_cast<LogLevel>(static_cast<int>(LogLevel::Count) + 1);
    LogMessage msg(invalid_level, "Unknown level");
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find("[Unknown  ]:"), std::string::npos);
    EXPECT_NE(formatted.find("\033[0m"), std::string::npos);
}

/**
 * @brief Tests that an explicitly empty message is still formatted correctly.
 */
TEST_F(SimpleFormatterTest, EmptyMessageFormattedCorrectly)
{
    SimpleFormatter formatter;
    LogMessage msg(LogLevel::Info, "");
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find("[Info     ]:"), std::string::npos);
    EXPECT_GT(formatted.size(), 20);
}

/**
 * @brief Tests that special characters in the message are handled and formatted as expected.
 */
TEST_F(SimpleFormatterTest, SpecialCharactersInMessage)
{
    SimpleFormatter formatter;
    std::string special_msg = "Line1\nLine2\tTab\\Backslash";
    LogMessage msg(LogLevel::Debug, special_msg);
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find("Line1\nLine2\tTab\\Backslash"), std::string::npos);
}

/**
 * @brief Tests that the expected ANSI color codes are present in the output for each log level.
 */
TEST_F(SimpleFormatterTest, ColorCodesPresentForEachLevel)
{
    SimpleFormatter formatter;
    struct LevelColor
    {
        LogLevel level;
        const char* color_code;
    } level_colors[] = {
        {LogLevel::Trace, "\033[96m"},
        {LogLevel::Debug, "\033[92m"},
        {LogLevel::Info, "\033[94m"},
        {LogLevel::Warning, "\033[93m"},
        {LogLevel::Error, "\033[91m"},
        {LogLevel::Fatal, "\033[95m"}
    };
    for (const auto& lc : level_colors)
    {
        LogMessage msg(lc.level, "Test");
        auto formatted = formatter.format(msg, std::source_location::current());
        EXPECT_NE(formatted.find(lc.color_code), std::string::npos);
    }
}

/**
 * @brief Tests that calling format without an explicit source_location provides valid context.
 */
TEST_F(SimpleFormatterTest, FormatWithDefaultSourceLocation)
{
    SimpleFormatter formatter;
    LogMessage msg(LogLevel::Info, "Default location");
    auto formatted = formatter.format(msg);
    EXPECT_NE(formatted.find("Default location"), std::string::npos);
    EXPECT_NE(formatted.find(".cpp"), std::string::npos);
}

/**
 * @brief Tests that a message with non-ASCII content in the message is handled correctly.
 */
TEST_F(SimpleFormatterTest, NonAsciiMessageContent)
{
    SimpleFormatter formatter;
    std::string non_ascii_msg = "Non-ASCII: üñîçødë 测试";
    LogMessage msg(LogLevel::Debug, non_ascii_msg);
    auto formatted = formatter.format(msg, std::source_location::current());
    EXPECT_NE(formatted.find("üñîçødë"), std::string::npos);
    EXPECT_NE(formatted.find("测试"), std::string::npos);
}
