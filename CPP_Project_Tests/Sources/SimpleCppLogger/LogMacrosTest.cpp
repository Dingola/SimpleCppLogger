#include "SimpleCppLogger/LogMacrosTest.h"

#include "SimpleCppLogger/Logger.h"

using namespace SimpleCppLogger;

/**
 * @brief Sets up the test fixture by clearing appenders and adding a mock appender.
 */
void LogMacrosTest::SetUp()
{
    m_mock_appender = std::make_shared<MockLogAppenderMacros>();
    Logger::get_instance().clear_appenders();
    Logger::get_instance().add_appender(m_mock_appender);
    Logger::get_instance().set_log_level(LogLevel::Trace);
}

/**
 * @brief Tears down the test fixture by clearing appenders.
 */
void LogMacrosTest::TearDown()
{
    Logger::get_instance().clear_appenders();
    m_mock_appender.reset();
}

/**
 * @brief Tests that LOG_DEBUG macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogDebugMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Debug);
            EXPECT_EQ(log_message.get_message(), "Debug message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_DEBUG << "Debug message";
}

/**
 * @brief Tests that LOG_INFO macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogInfoMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Info);
            EXPECT_EQ(log_message.get_message(), "Info message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_INFO << "Info message";
}

/**
 * @brief Tests that LOG_WARNING macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogWarningMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Warning);
            EXPECT_EQ(log_message.get_message(), "Warning message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_WARNING << "Warning message";
}

/**
 * @brief Tests that LOG_ERROR macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogErrorMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Error);
            EXPECT_EQ(log_message.get_message(), "Error message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_ERROR << "Error message";
}

/**
 * @brief Tests that LOG_FATAL macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogFatalMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Fatal);
            EXPECT_EQ(log_message.get_message(), "Fatal message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_FATAL << "Fatal message";
}

/**
 * @brief Tests that LOG_TRACE macro logs with correct level and message.
 */
TEST_F(LogMacrosTest, LogTraceMacro)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Trace);
            EXPECT_EQ(log_message.get_message(), "Trace message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LOG_TRACE << "Trace message";
}

/**
 * @brief Tests that macros support streaming multiple values.
 */
TEST_F(LogMacrosTest, LogMacroMultipleValues)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), "Value: 123, Pi: 3.14, Text: test");
        });

    LOG_INFO << "Value: " << 123 << ", Pi: " << 3.14 << ", Text: " << "test";
}

/**
 * @brief Tests that macros support stream manipulators.
 */
TEST_F(LogMacrosTest, LogMacroWithStreamManipulator)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), std::string("Line1\nLine2"));
        });

    LOG_INFO << "Line1" << std::endl << "Line2";
}

/**
 * @brief Tests that macros provide correct source location in a helper function.
 */
void helper_func_for_macro_source_location(MockLogAppenderMacros* appender)
{
    EXPECT_CALL(*appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage&, const std::source_location& location) {
            std::string func_name = location.function_name();
            EXPECT_NE(func_name.find("helper_func_for_macro_source_location"), std::string::npos);
        });
    LOG_INFO << "From helper";
}

TEST_F(LogMacrosTest, MacroSourceLocationFromDifferentFunction)
{
    helper_func_for_macro_source_location(m_mock_appender.get());
}

/**
 * @brief Tests that macros do not log if log level is filtered.
 */
TEST_F(LogMacrosTest, MacroLogLevelFiltersMessages)
{
    Logger::get_instance().set_log_level(LogLevel::Error);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    LOG_INFO << "Should not be logged";
    LOG_ERROR << "Should be logged";
}

/**
 * @brief Tests that macros handle empty messages.
 */
TEST_F(LogMacrosTest, MacroLogEmptyMessage)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), "");
        });

    LOG_INFO;
}

/**
 * @brief Tests that macros handle very long messages.
 */
TEST_F(LogMacrosTest, MacroLogVeryLongMessage)
{
    std::string long_message(10000, 'X');
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([&](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), long_message);
        });

    LOG_INFO << long_message;
}
