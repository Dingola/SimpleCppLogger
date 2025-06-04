#pragma once

#include <gtest/gtest.h>

/**
 * @file SimpleFormatterTest.h
 * @brief Test fixture for SimpleCppLogger::SimpleFormatter.
 */

class SimpleFormatterTest: public ::testing::Test
{
    protected:
        SimpleFormatterTest() = default;
        ~SimpleFormatterTest() override = default;

        void SetUp() override {}
        void TearDown() override {}
};
