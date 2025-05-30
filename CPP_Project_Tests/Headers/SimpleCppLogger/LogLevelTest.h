#pragma once

#include <gtest/gtest.h>

#include "SimpleCppLogger/LogLevel.h"

/**
 * @file LogLevelTest.h
 * @brief Test fixture for SimpleCppLogger::LogLevel and its utilities.
 */

class LogLevelTest: public ::testing::Test
{
    protected:
        LogLevelTest() = default;
        ~LogLevelTest() override = default;

        void SetUp() override {}
        void TearDown() override {}
};
