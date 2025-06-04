#include "SimpleCppLogger/LogMessageTest.h"

#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/LogMessage.h"

using namespace SimpleCppLogger;

/**
 * @brief Tests that the default constructor sets level to Info and message to empty.
 */
TEST_F(LogMessageTest, DefaultConstructor)
{
    LogMessage msg;
    EXPECT_EQ(msg.get_level(), LogLevel::Info);
    EXPECT_EQ(msg.get_message(), "");
}

/**
 * @brief Tests that a custom log level and message are set correctly.
 */
TEST_F(LogMessageTest, CustomLevelAndMessage)
{
    LogMessage msg(LogLevel::Error, "Fehler aufgetreten");
    EXPECT_EQ(msg.get_level(), LogLevel::Error);
    EXPECT_EQ(msg.get_message(), "Fehler aufgetreten");
}

/**
 * @brief Tests move semantics for the message string.
 */
TEST_F(LogMessageTest, MoveMessage)
{
    std::string text = "Moved message";
    LogMessage msg(LogLevel::Debug, std::move(text));
    EXPECT_EQ(msg.get_level(), LogLevel::Debug);
    EXPECT_EQ(msg.get_message(), "Moved message");
    EXPECT_TRUE(text.empty() || text == "Moved message");  // je nach STL-Implementierung
}

/**
 * @brief Tests that an explicitly empty message is stored correctly.
 */
TEST_F(LogMessageTest, ExplicitEmptyMessage)
{
    LogMessage msg(LogLevel::Warning, "");
    EXPECT_EQ(msg.get_level(), LogLevel::Warning);
    EXPECT_EQ(msg.get_message(), "");
}

/**
 * @brief Tests that a very long message is stored correctly.
 */
TEST_F(LogMessageTest, LongMessage)
{
    std::string long_msg(10000, 'x');
    LogMessage msg(LogLevel::Info, long_msg);
    EXPECT_EQ(msg.get_message(), long_msg);
}

/**
 * @brief Tests that a UTF-8 encoded message is stored and retrieved correctly.
 */
TEST_F(LogMessageTest, Utf8Message)
{
    std::string utf8_msg = "Grüße 🌍";
    LogMessage msg(LogLevel::Debug, utf8_msg);
    EXPECT_EQ(msg.get_message(), utf8_msg);
}

/**
 * @brief Tests the copy constructor for LogMessage.
 */
TEST_F(LogMessageTest, CopyConstructor)
{
    LogMessage original(LogLevel::Error, "Copy me");
    LogMessage copy = original;
    EXPECT_EQ(copy.get_level(), LogLevel::Error);
    EXPECT_EQ(copy.get_message(), "Copy me");
}

/**
 * @brief Tests the copy assignment operator for LogMessage.
 */
TEST_F(LogMessageTest, CopyAssignment)
{
    LogMessage original(LogLevel::Fatal, "Assign me");
    LogMessage copy;
    copy = original;
    EXPECT_EQ(copy.get_level(), LogLevel::Fatal);
    EXPECT_EQ(copy.get_message(), "Assign me");
}

/**
 * @brief Tests the move constructor for LogMessage.
 */
TEST_F(LogMessageTest, MoveConstructor)
{
    LogMessage original(LogLevel::Trace, "Move me");
    LogMessage moved = std::move(original);
    EXPECT_EQ(moved.get_level(), LogLevel::Trace);
    EXPECT_EQ(moved.get_message(), "Move me");
}

/**
 * @brief Tests the move assignment operator for LogMessage.
 */
TEST_F(LogMessageTest, MoveAssignment)
{
    LogMessage original(LogLevel::Debug, "Move assign me");
    LogMessage moved;
    moved = std::move(original);
    EXPECT_EQ(moved.get_level(), LogLevel::Debug);
    EXPECT_EQ(moved.get_message(), "Move assign me");
}

/**
 * @brief Tests that all LogLevel values are set and retrieved correctly.
 */
TEST_F(LogMessageTest, AllLogLevels)
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(LogLevel::Count); ++i)
    {
        LogLevel level = static_cast<LogLevel>(i);
        LogMessage msg(level, "Test");
        EXPECT_EQ(msg.get_level(), level);
    }
}
