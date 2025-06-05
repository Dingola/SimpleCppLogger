/// @file LogStream.cpp
/// @brief Implements the LogStream class.

#include "SimpleCppLogger/LogStream.h"

#include "SimpleCppLogger/Logger.h"

namespace SimpleCppLogger
{

LogStream::LogStream(LogLevel level, std::source_location location)
    : m_level(level), m_location(location)
{}

LogStream& LogStream::operator<<(std::ostream& (*manip)(std::ostream&))
{
    m_stream << manip;
    return *this;
}

LogStream::~LogStream()
{
    Logger::get_instance().log(m_level, m_stream.str(), m_location);
}

}  // namespace SimpleCppLogger
