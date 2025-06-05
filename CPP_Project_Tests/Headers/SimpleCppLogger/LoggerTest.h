#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <source_location>
#include <string>

#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/Logger.h"

/**
 * @file LoggerTest.h
 * @brief Test fixture for SimpleCppLogger::Logger and its core functionality.
 */

class MockLogAppender: public SimpleCppLogger::LogAppender
{
    public:
        MockLogAppender(): LogAppender() {}

        MOCK_METHOD(void, internal_append,
                    (const SimpleCppLogger::LogMessage& message,
                     const std::source_location& location),
                    (override));
};

class LoggerTest: public ::testing::Test
{
    protected:
        LoggerTest() = default;
        ~LoggerTest() override = default;

        void SetUp() override;
        void TearDown() override;

        std::shared_ptr<MockLogAppender> m_mock_appender;
};
