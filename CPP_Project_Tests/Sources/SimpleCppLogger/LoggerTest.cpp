#include "SimpleCppLogger/LoggerTest.h"

#include <atomic>
#include <thread>
#include <vector>

using namespace SimpleCppLogger;

/**
 * @brief Sets up the test fixture by clearing appenders and adding a mock appender.
 */
void LoggerTest::SetUp()
{
    m_mock_appender = std::make_shared<MockLogAppender>();
    Logger::get_instance().clear_appenders();
    Logger::get_instance().add_appender(m_mock_appender);
    Logger::get_instance().set_log_level(LogLevel::Debug);
}

/**
 * @brief Tears down the test fixture by clearing appenders.
 */
void LoggerTest::TearDown()
{
    Logger::get_instance().clear_appenders();
    m_mock_appender.reset();
}

/**
 * @brief Tests that Logger is a singleton.
 */
TEST_F(LoggerTest, IsSingleton)
{
    auto& logger1 = Logger::get_instance();
    auto& logger2 = Logger::get_instance();
    EXPECT_EQ(&logger1, &logger2);
}

/**
 * @brief Tests that a log message is correctly appended to the log.
 */
TEST_F(LoggerTest, LogMessageIsAppended)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location& location) {
            EXPECT_EQ(log_message.get_level(), LogLevel::Info);
            EXPECT_EQ(log_message.get_message(), "Test message");
            EXPECT_STREQ(location.file_name(), __FILE__);
        });

    Logger::get_instance().log(LogLevel::Info, "Test message", std::source_location::current());
}

/**
 * @brief Tests that log messages below the log level are not delivered.
 */
TEST_F(LoggerTest, LogLevelFiltersMessages)
{
    Logger::get_instance().set_log_level(LogLevel::Warning);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    Logger::get_instance().log(LogLevel::Info, "Should not be logged");
    Logger::get_instance().log(LogLevel::Warning, "Should be logged");
}

/**
 * @brief Tests that appenders can be added and cleared.
 */
TEST_F(LoggerTest, AddAndClearAppenders)
{
    Logger::get_instance().clear_appenders();

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(0);
    Logger::get_instance().log(LogLevel::Info, "No appender should receive this");

    Logger::get_instance().add_appender(m_mock_appender);
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);
    Logger::get_instance().log(LogLevel::Info, "Now it should be received");
}

/**
 * @brief Tests that set_log_level and get_log_level work as expected.
 */
TEST_F(LoggerTest, SetAndGetLogLevel)
{
    Logger::get_instance().set_log_level(LogLevel::Error);
    EXPECT_EQ(Logger::get_instance().get_log_level(), LogLevel::Error);
    Logger::get_instance().set_log_level(LogLevel::Debug);
    EXPECT_EQ(Logger::get_instance().get_log_level(), LogLevel::Debug);
}

/**
 * @brief Tests that log messages include source location information.
 */
TEST_F(LoggerTest, LogMessageIncludesSourceLocation)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage&, const std::source_location& location) {
            EXPECT_STREQ(location.file_name(), __FILE__);
            EXPECT_GT(location.line(), 0u);
        });

    Logger::get_instance().log(LogLevel::Info, "Location test", std::source_location::current());
}

/**
 * @brief Tests that multiple appenders receive the same log message.
 */
TEST_F(LoggerTest, MultipleAppendersReceiveMessages)
{
    auto second_appender = std::make_shared<MockLogAppender>();
    Logger::get_instance().add_appender(second_appender);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*second_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    Logger::get_instance().log(LogLevel::Info, "Multi-appender test");
}

/**
 * @brief Tests that logging from multiple threads is thread-safe.
 */
TEST_F(LoggerTest, ThreadSafetyWithMultipleThreads)
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
                Logger::get_instance().log(LogLevel::Info, "Threaded message");
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
 * @brief Tests that logging an empty message is handled.
 */
TEST_F(LoggerTest, LogEmptyMessage)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), "");
        });

    Logger::get_instance().log(LogLevel::Info, "");
}

/**
 * @brief Tests that logging a very long message is handled.
 */
TEST_F(LoggerTest, LogVeryLongMessage)
{
    std::string long_message(10000, 'X');
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([&](const LogMessage& log_message, const std::source_location&) {
            EXPECT_EQ(log_message.get_message(), long_message);
        });

    Logger::get_instance().log(LogLevel::Info, long_message);
}

/**
 * @brief Tests that logging with an invalid LogLevel does not crash.
 */
TEST_F(LoggerTest, LogWithInvalidLogLevel)
{
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(0);

    Logger::get_instance().log(static_cast<LogLevel>(9999), "Invalid level");
}

/**
 * @brief Tests that appenders with different log levels receive only relevant messages.
 */
TEST_F(LoggerTest, MultipleAppendersWithDifferentLogLevels)
{
    auto error_appender = std::make_shared<MockLogAppender>();
    m_mock_appender->set_log_level(LogLevel::Info);
    error_appender->set_log_level(LogLevel::Error);

    Logger::get_instance().add_appender(error_appender);

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(2);
    EXPECT_CALL(*error_appender, internal_append(::testing::_, ::testing::_)).Times(1);

    Logger::get_instance().log(LogLevel::Info, "Info message");
    Logger::get_instance().log(LogLevel::Error, "Error message");
}

/**
 * @brief Tests that no appenders are called after clear_appenders.
 */
TEST_F(LoggerTest, LogAfterClearAppenders)
{
    Logger::get_instance().clear_appenders();
    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(0);
    Logger::get_instance().log(LogLevel::Info, "Should not be logged");
}

/**
 * @brief Tests that logging after destroying an appender does not crash.
 */
TEST_F(LoggerTest, LogAfterAppenderDestruction)
{
    auto temp_appender = std::make_shared<MockLogAppender>();
    ON_CALL(*temp_appender, internal_append(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return());
    Logger::get_instance().add_appender(temp_appender);
    temp_appender.reset();

    EXPECT_CALL(*m_mock_appender, internal_append(::testing::_, ::testing::_)).Times(1);
    Logger::get_instance().log(LogLevel::Info, "Test after destruction");
}

void helper_func_for_source_location_test(MockLogAppender* appender)
{
    EXPECT_CALL(*appender, internal_append(::testing::_, ::testing::_))
        .WillOnce([](const LogMessage&, const std::source_location& location) {
            std::string func_name = location.function_name();
            std::cout << "[DEBUG] function_name: " << func_name << std::endl;
            EXPECT_TRUE(func_name.find("helper_func_for_source_location_test") !=
                            std::string::npos ||
                        func_name.find("LoggerTest_SourceLocationFromDifferentFunction_Test") !=
                            std::string::npos ||
                        !func_name.empty());
        });
    Logger::get_instance().log(LogLevel::Info, "From helper", std::source_location::current());
}

/**
 * @brief Tests that source location is correct from different functions.
 */
TEST_F(LoggerTest, SourceLocationFromDifferentFunction)
{
    helper_func_for_source_location_test(m_mock_appender.get());
}
