#include "SimpleCppLogger/LogLevelTest.h"

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

using namespace SimpleCppLogger;

/**
 * @brief Tests that LogLevel enum values are sequential and match the expected count.
 */
TEST_F(LogLevelTest, EnumValueCountAndOrder)
{
    EXPECT_EQ(static_cast<std::size_t>(LogLevel::Count), LogLevelCount);
    EXPECT_EQ(LogLevelNamesA[0], std::string("Trace"));
    EXPECT_EQ(LogLevelNamesA[LogLevelCount - 1], std::string("Fatal"));
}

/**
 * @brief Tests that to_string returns the correct narrow string for each LogLevel.
 */
TEST_F(LogLevelTest, ToStringReturnsCorrectNarrowString)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        EXPECT_STREQ(to_string(level), LogLevelNamesA[index]);
    }
}

/**
 * @brief Tests that to_wstring returns the correct wide string for each LogLevel.
 */
TEST_F(LogLevelTest, ToWStringReturnsCorrectWideString)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        EXPECT_STREQ(to_wstring(level), LogLevelNamesW[index]);
    }
}

/**
 * @brief Tests that to_string_view returns the correct std::string_view for each LogLevel.
 */
TEST_F(LogLevelTest, ToStringViewReturnsCorrectStringView)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        EXPECT_EQ(to_string_view(level), LogLevelNamesSv[index]);
    }
}

/**
 * @brief Tests that to_wstring_view returns the correct std::wstring_view for each LogLevel.
 */
TEST_F(LogLevelTest, ToWStringViewReturnsCorrectWStringView)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        EXPECT_EQ(to_wstring_view(level), LogLevelNamesWsv[index]);
    }
}

/**
 * @brief Tests that from_string_view returns the correct LogLevel for valid strings.
 */
TEST_F(LogLevelTest, FromStringViewReturnsCorrectLogLevel)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        std::string_view string_view_value = LogLevelNamesSv[index];
        std::optional<LogLevel> log_level_optional = from_string_view(string_view_value);
        ASSERT_TRUE(log_level_optional.has_value());
        EXPECT_EQ(static_cast<std::size_t>(log_level_optional.value()), index);
    }
}

/**
 * @brief Tests that from_wstring_view returns the correct LogLevel for valid wide strings.
 */
TEST_F(LogLevelTest, FromWStringViewReturnsCorrectLogLevel)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        std::wstring_view wstring_view_value = LogLevelNamesWsv[index];
        std::optional<LogLevel> log_level_optional = from_wstring_view(wstring_view_value);
        ASSERT_TRUE(log_level_optional.has_value());
        EXPECT_EQ(static_cast<std::size_t>(log_level_optional.value()), index);
    }
}

/**
 * @brief Tests that from_string_view returns std::nullopt for invalid input.
 */
TEST_F(LogLevelTest, FromStringViewReturnsNulloptForInvalid)
{
    EXPECT_FALSE(from_string_view("NotALevel").has_value());
    EXPECT_FALSE(from_string_view("").has_value());
}

/**
 * @brief Tests that from_wstring_view returns std::nullopt for invalid input.
 */
TEST_F(LogLevelTest, FromWStringViewReturnsNulloptForInvalid)
{
    EXPECT_FALSE(from_wstring_view(L"NotALevel").has_value());
    EXPECT_FALSE(from_wstring_view(L"").has_value());
}

/**
 * @brief Tests that to_string and to_wstring return empty string for out-of-range LogLevel.
 */
TEST_F(LogLevelTest, ToStringReturnsEmptyForOutOfRange)
{
    LogLevel invalid = static_cast<LogLevel>(LogLevelCount);
    EXPECT_STREQ(to_string(invalid), "");
    EXPECT_STREQ(to_wstring(invalid), L"");
    EXPECT_EQ(to_string_view(invalid), std::string_view{});
    EXPECT_EQ(to_wstring_view(invalid), std::wstring_view{});
}

/**
 * @brief Tests that LogLevel enum values can be compared by severity.
 */
TEST_F(LogLevelTest, EnumComparisonWorks)
{
    EXPECT_TRUE(LogLevel::Fatal > LogLevel::Error);
    EXPECT_TRUE(LogLevel::Error > LogLevel::Warning);
    EXPECT_TRUE(LogLevel::Warning > LogLevel::Info);
    EXPECT_TRUE(LogLevel::Info > LogLevel::Debug);
    EXPECT_TRUE(LogLevel::Debug > LogLevel::Trace);

    EXPECT_TRUE(LogLevel::Trace < LogLevel::Debug);
    EXPECT_TRUE(LogLevel::Info >= LogLevel::Info);
    EXPECT_TRUE(LogLevel::Warning <= LogLevel::Warning);
    EXPECT_FALSE(LogLevel::Fatal < LogLevel::Trace);
    EXPECT_TRUE(LogLevel::Error != LogLevel::Warning);
    EXPECT_TRUE(LogLevel::Debug == LogLevel::Debug);
}

/**
 * @brief Tests all comparison operators for LogLevel.
 */
TEST_F(LogLevelTest, AllComparisonOperators)
{
    EXPECT_TRUE(LogLevel::Fatal == LogLevel::Fatal);
    EXPECT_TRUE(LogLevel::Fatal != LogLevel::Error);
    EXPECT_TRUE(LogLevel::Debug < LogLevel::Error);
    EXPECT_TRUE(LogLevel::Debug <= LogLevel::Error);
    EXPECT_TRUE(LogLevel::Error > LogLevel::Debug);
    EXPECT_TRUE(LogLevel::Error >= LogLevel::Debug);
}

/**
 * @brief Tests that all integer values in range are valid LogLevel values.
 */
TEST_F(LogLevelTest, AllEnumValuesAreValid)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        EXPECT_NE(to_string(level), nullptr);
        EXPECT_NE(to_wstring(level), nullptr);
    }
}

/**
 * @brief Tests roundtrip conversion: LogLevel -> string_view -> LogLevel.
 */
TEST_F(LogLevelTest, RoundtripEnumStringConversion)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        LogLevel level = static_cast<LogLevel>(index);
        std::string_view string_view_value = to_string_view(level);
        std::optional<LogLevel> log_level_optional = from_string_view(string_view_value);
        ASSERT_TRUE(log_level_optional.has_value());
        EXPECT_EQ(log_level_optional.value(), level);
    }
}

/**
 * @brief Tests that out-of-range enum values are handled gracefully.
 */
TEST_F(LogLevelTest, OutOfRangeEnumValues)
{
    LogLevel invalid_low = static_cast<LogLevel>(-1);
    LogLevel invalid_high = static_cast<LogLevel>(LogLevelCount + 10);
    EXPECT_STREQ(to_string(invalid_low), "");
    EXPECT_STREQ(to_string(invalid_high), "");
    EXPECT_EQ(to_string_view(invalid_low), std::string_view{});
    EXPECT_EQ(to_string_view(invalid_high), std::string_view{});
}

/**
 * @brief Tests that from_string_view is case-sensitive.
 */
TEST_F(LogLevelTest, FromStringViewIsCaseSensitive)
{
    EXPECT_FALSE(from_string_view("fatal").has_value());
    EXPECT_FALSE(from_string_view("ERROR").has_value());
    EXPECT_TRUE(from_string_view("Fatal").has_value());
}

/**
 * @brief Tests that narrow and wide string representations are consistent.
 */
TEST_F(LogLevelTest, NarrowAndWideStringConsistency)
{
    for (std::size_t index = 0; index < LogLevelCount; ++index)
    {
        std::string narrow(LogLevelNamesA[index]);
        std::wstring wide(LogLevelNamesW[index]);
        EXPECT_EQ(std::wstring(narrow.begin(), narrow.end()), wide);
    }
}
