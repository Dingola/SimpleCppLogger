#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <source_location>
#include <string>

#include "SimpleCppLogger/LogAppender.h"
#include "SimpleCppLogger/LogLevel.h"
#include "SimpleCppLogger/LogMacros.h"

/**
 * @file LogMacrosTest.h
 * @brief Test fixture for the logging macros in SimpleCppLogger/LogMacros.h.
 */

class MockLogAppenderMacros: public SimpleCppLogger::LogAppender
{
    public:
        MockLogAppenderMacros(): LogAppender() {}

        MOCK_METHOD(void, internal_append,
                    (const SimpleCppLogger::LogMessage& message,
                     const std::source_location& location),
                    (override));
};

class LogMacrosTest: public ::testing::Test
{
    protected:
        LogMacrosTest() = default;
        ~LogMacrosTest() override = default;

        void SetUp() override;
        void TearDown() override;

        std::shared_ptr<MockLogAppenderMacros> m_mock_appender;
};
