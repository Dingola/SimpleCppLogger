#pragma once

#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <sstream>

#include "SimpleCppLogger/ConsoleAppender.h"
#include "SimpleCppLogger/LogMessage.h"
#include "SimpleCppLogger/SimpleFormatter.h"

/**
 * @file ConsoleAppenderTest.h
 * @brief Test fixture for SimpleCppLogger::ConsoleAppender.
 */
class ConsoleAppenderTest: public ::testing::Test
{
    protected:
        ConsoleAppenderTest() = default;
        ~ConsoleAppenderTest() override = default;

        void SetUp() override;
        void TearDown() override;

        static auto capture_console_output(std::function<void()> func) -> std::string;

        std::shared_ptr<SimpleCppLogger::ConsoleAppender> m_console_appender;
        std::streambuf* m_cout_buf = nullptr;
        std::streambuf* m_cerr_buf = nullptr;
        static std::ostringstream m_cout_stream;
        static std::ostringstream m_cerr_stream;
};
