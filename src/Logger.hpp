#pragma once

#include <iosfwd>
#include <source_location>
#include <string_view>

#include "Colors.hpp"

namespace Log {

enum class Level : int {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
};

class Logger
{
    using LogSink = std::ostream;
public:
    Logger(LogSink &sink) : m_sink {sink} {}

    Logger();

    void
    log(const Log::Level           level,
        const std::string_view     message,
        const std::source_location source = std::source_location::current());

    void
    log(const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Info, message, source);
    }

    ~Logger();
private:
    LogSink &m_sink;
};

}    // namespace Log

inline Log::Logger logger;