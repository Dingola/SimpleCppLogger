#pragma once

#include <gtest/gtest.h>

/**
 * @file LogMessageTest.h
 * @brief Test fixture for SimpleCppLogger::LogMessage
 */

class LogMessageTest: public ::testing::Test
{
    protected:
        LogMessageTest() = default;
        ~LogMessageTest() override = default;

        void SetUp() override {}
        void TearDown() override {}
};
