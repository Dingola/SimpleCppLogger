/// @file LogStream.h
/// @brief Defines the LogStream class for stream-style logging.

#pragma once

#include <source_location>
#include <sstream>
#include <string>

#include "SimpleCppLogger/LogLevel.h"

namespace SimpleCppLogger
{
/**
 * @class LogStream
 * @brief Helper class for stream-style logging.
 *
 * This class allows logging messages using the stream operator (<<).
 * The message is sent to the logger when the LogStream object is destroyed.
 */
class LogStream
{
    public:
        /**
         * @brief Constructs a LogStream for the given log level and source location.
         * @param level The log level.
         * @param location The source location (defaults to caller location).
         */
        LogStream(LogLevel level, std::source_location location = std::source_location::current());

        /**
         * @brief Appends a value to the log message.
         * @tparam T The type of the value.
         * @param value The value to append.
         * @return Reference to this LogStream.
         */
        template<typename T>
        LogStream& operator<<(const T& value)
        {
            m_stream << value;
            return *this;
        }

        /**
         * @brief Appends a stream manipulator (e.g., std::endl) to the log message.
         * @param manip The stream manipulator.
         * @return Reference to this LogStream.
         */
        LogStream& operator<<(std::ostream& (*manip)(std::ostream&));

        /**
         * @brief Destructor. Sends the accumulated message to the logger.
         */
        ~LogStream();

    private:
        LogLevel m_level;
        std::ostringstream m_stream;
        std::source_location m_location;
};

}  // namespace SimpleCppLogger
