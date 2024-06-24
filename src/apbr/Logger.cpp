#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <string_view>
#include <iostream>
#include <format>
#include <chrono>
#include <source_location>

#include <apbr/Logger.hpp>
#include <apbr/color.hpp>

namespace {

auto as_localTime(const std::chrono::system_clock::time_point &tp =
                      std::chrono::system_clock::now())
{
    return std::chrono::zoned_time {std::chrono::current_zone(), tp};
}

auto current_time()
{
    return std::chrono::system_clock::now();
}

std::string to_string(const std::chrono::system_clock::time_point &tp)
{
    return std::format("{:%F %T %Z}", as_localTime(tp));
}

std::string to_string(const std::source_location source)
{
    return std::format("{}: ({}:{}) `{}`",
                       source.file_name(),
                       source.line(),
                       source.column(),
                       source.function_name());
}

std::string to_string(const apbr::Log::Level level)
{
    switch (level) {
        using enum apbr::Log::Level;
    case Trace:
        return "Trace";
    case Debug:
        return "Debug";
    case Info:
        return "Info";
    case Warn:
        return "Warn";
    case Error:
        return "Error";
    case Fatal:
        return "Fatal";
    default:
        return "??";
    }
}

std::string LogLevelColor(const apbr::Log::Level level)
{
    switch (level) {
        using enum apbr::Log::Level;
        using namespace apbr;
    case Trace:
        return color::trace;
    case Debug:
        return color::debug;
    case Info:
        return color::info;
    case Warn:
        return color::warn;
    case Error:
        return color::error;
    case Fatal:
        return color::fatal;
    default:
        return color::reset;
    }
}

std::string formatLogData(const apbr::Log::Level     level,
                          const std::string_view     message,
                          const std::source_location source)
{
    return std::format("[{}] {} | {} | {}",
                       to_string(level),
                       to_string(current_time()),
                       to_string(source),
                       message);
}

}    // namespace

namespace apbr::Log {

void Logger::log(const apbr::Log::Level     level,
                 const std::string_view     message,
                 const std::source_location source)
{
#if defined(NDEBUG)
    if (static_cast<int>(level) < static_cast<int>(Level::Warn))
        return;
#endif

    m_sink << LogLevelColor(level) << formatLogData(level, message, source)
           << '\n'
           << apbr::color::reset;
}

Logger::Logger() : m_sink {std::clog}
{
}

Logger::~Logger()
{
    // output all the log messages before the object is destroyed.
    m_sink << m_sink.rdbuf();
    m_sink.flush();
}

}    // namespace apbr::Log