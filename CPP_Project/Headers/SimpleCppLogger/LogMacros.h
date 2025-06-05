#pragma once
#include "SimpleCppLogger/LogStream.h"

#define LOG_DEBUG                                                    \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Debug, \
                                 std::source_location::current())
#define LOG_INFO \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Info, std::source_location::current())
#define LOG_WARNING                                                    \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Warning, \
                                 std::source_location::current())
#define LOG_ERROR                                                    \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Error, \
                                 std::source_location::current())
#define LOG_FATAL                                                    \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Fatal, \
                                 std::source_location::current())
#define LOG_TRACE                                                    \
    ::SimpleCppLogger::LogStream(::SimpleCppLogger::LogLevel::Trace, \
                                 std::source_location::current())
