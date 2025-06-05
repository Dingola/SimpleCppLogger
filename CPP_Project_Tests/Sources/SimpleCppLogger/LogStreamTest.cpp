#include "SimpleCppLogger/LogStreamTest.h"

#include "SimpleCppLogger/Logger.h"

using namespace SimpleCppLogger;

/**
 * @brief Sets up the test fixture by clearing appenders and adding a mock appender.
 */
void LogStreamTest::SetUp()
{
    m_mock_appender = std::make_shared<MockLogAppenderStream>();
    Logger::get_instance().clear_appenders();
    Logger::get_instance().add_appender(m_mock_appender);
    Logger::get_instance().set_log_level(LogLevel::Debug);
}

/**
 * @brief Tears down the test fixture by clearing appenders.
 */
void LogStreamTest::TearDown()
{
    Logger::get_instance().clear_appenders();
    m_mock_appender.reset();
}

/**
 * @brief Tests that a simple message is logged via LogStream.
 */
TEST_F(LogStreamTest, LogSimpleMessage)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Info);
            EXPECT_EQ(log_message.get_message(), "Stream message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    LogStream(LogLevel::Info) << "Stream message";
}

/**
 * @brief Tests that multiple values can be streamed and concatenated.
 */
TEST_F(LogStreamTest, LogMultipleValues)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), "Value: 42, Pi: 3.14, Text: test");
        });

    LogStream(LogLevel::Info) << "Value: " << 42 << ", Pi: " << 3.14 << ", Text: " << "test";
}

/**
 * @brief Tests that stream manipulators (e.g. std::endl) are handled.
 */
TEST_F(LogStreamTest, LogWithStreamManipulator)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            // std::endl inserts '\n'
            EXPECT_EQ(log_message.get_message(), std::string("Line1\nLine2"));
        });

    LogStream(LogLevel::Info) << "Line1" << std::endl << "Line2";
}

/**
 * @brief Tests that log messages below the log level are not delivered.
 */
TEST_F(LogStreamTest, LogLevelFiltersMessages)
{
    Logger::get_instance().set_log_level(LogLevel::Warning);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    LogStream(LogLevel::Info) << "Should not be logged";
    LogStream(LogLevel::Warning) << "Should be logged";
}

/**
 * @brief Tests that log messages include source location information.
 */
TEST_F(LogStreamTest, LogMessageIncludesSourceLocation)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage&, const std::source_location& location) {
            EXPECT_STREQ(location.file_name(), __FILE__);
            EXPECT_GT(location.line(), 0u);
        });

    LogStream(LogLevel::Info) << "Location test";
}

/**
 * @brief Tests that logging an empty message is handled.
 */
TEST_F(LogStreamTest, LogEmptyMessage)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), "");
        });

    LogStream(LogLevel::Info) << "";
}

/**
 * @brief Tests that logging a very long message is handled.
 */
TEST_F(LogStreamTest, LogVeryLongMessage)
{
    std::string long_message(10000, 'X');
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([&](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), long_message);
        });

    LogStream(LogLevel::Info) << long_message;
}

/**
 * @brief Tests that logging with an invalid LogLevel does not crash.
 */
TEST_F(LogStreamTest, LogWithInvalidLogLevel)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(0);

    LogStream(static_cast<LogLevel>(9999)) << "Invalid level";
}

/**
 * @brief Tests that multiple appenders receive the same log message.
 */
TEST_F(LogStreamTest, MultipleAppendersReceiveMessages)
{
    auto second_appender = std::make_shared<MockLogAppenderStream>();
    Logger::get_instance().add_appender(second_appender);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*second_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    LogStream(LogLevel::Info) << "Multi-appender test";
}

/**
 * @brief Tests that appenders with different log levels receive only relevant messages.
 */
TEST_F(LogStreamTest, MultipleAppendersWithDifferentLogLevels)
{
    auto error_appender = std::make_shared<MockLogAppenderStream>();
    m_mock_appender->set_log_level(LogLevel::Info);
    error_appender->set_log_level(LogLevel::Error);

    Logger::get_instance().add_appender(error_appender);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(2);
    EXPECT_CALL(*error_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    LogStream(LogLevel::Info) << "Info message";
    LogStream(LogLevel::Error) << "Error message";
}

/**
 * @brief Tests that no appenders are called after clear_appenders.
 */
TEST_F(LogStreamTest, LogAfterClearAppenders)
{
    Logger::get_instance().clear_appenders();
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(0);
    LogStream(LogLevel::Info) << "Should not be logged";
}

/**
 * @brief Tests that logging after destroying an appender does not crash.
 */
TEST_F(LogStreamTest, LogAfterAppenderDestruction)
{
    auto temp_appender = std::make_shared<MockLogAppenderStream>();
    Logger::get_instance().add_appender(temp_appender);
    temp_appender.reset();

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);
    LogStream(LogLevel::Info) << "Test after destruction";
}

/**
 * @brief Tests that logging from multiple threads is thread-safe.
 */
TEST_F(LogStreamTest, ThreadSafetyWithMultipleThreads)
{
    constexpr int thread_count = 8;
    constexpr int messages_per_thread = 100;
    std::atomic<int> call_count{0};

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .Times(thread_count * messages_per_thread)
        .WillRepeatedly([&](const LogMessage&, const std::source_location&) { ++call_count; });

    std::vector<std::thread> threads;

    for (int t = 0; t < thread_count; ++t)
    {
        threads.emplace_back([=, &call_count]() {
            for (int i = 0; i < messages_per_thread; ++i)
            {
                LogStream(LogLevel::Info) << "Threaded message";
            }
        });
    }

    for (auto& th: threads)
    {
        th.join();
    }

    EXPECT_EQ(call_count, thread_count * messages_per_thread);
}

/**
 * @brief Tests that source location is correct from different functions.
 */
void helper_func_for_source_location_stream_test(MockLogAppenderStream* appender)
{
    EXPECT_CALL(*appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage&, const std::source_location& location) {
            std::string func_name = location.function_name();
            EXPECT_NE(func_name.find("helper_func_for_source_location_stream_test"),
                      std::string::npos);
        });
    LogStream(LogLevel::Info) << "From helper";
}

TEST_F(LogStreamTest, SourceLocationFromDifferentFunction)
{
    helper_func_for_source_location_stream_test(m_mock_appender.get());
}
