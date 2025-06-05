#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <source_location>
#include <string>

#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/LogStream.h"

/**
 * @file LogStreamTest.h
 * @brief Test fixture for SimpleCppLogger::LogStream and its core functionality.
 */

class MockLogAppenderStream: public SimpleCppLogger::LogAppender
{
    public:
        MockLogAppenderStream(): LogAppender() {}

        MOCK_METHOD(void, internal_append,
                    (const SimpleCppLogger::LogMessage& message,
                     const std::source_location& location),
                    (override));
};

class LogStreamTest: public ::testing::Test
{
    protected:
        LogStreamTest() = default;
        ~LogStreamTest() override = default;

        void SetUp() override;
        void TearDown() override;

        std::shared_ptr<MockLogAppenderStream> m_mock_appender;
};
