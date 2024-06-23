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

    // general logging by default without supplying a log level always logs as `Log::Level::Info`.
    void
    log(const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Info, message, source);
    }

    void logFatal(
        const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Fatal, message, source);
    }

    void logError(
        const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Error, message, source);
    }

    void
    logWarn(const std::string_view     message,
            const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Warn, message, source);
    }

    void
    logInfo(const std::string_view     message,
            const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Info, message, source);
    }

    void logDebug(
        const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Debug, message, source);
    }

    void logTrace(
        const std::string_view     message,
        const std::source_location source = std::source_location::current())
    {
        log(Log::Level::Trace, message, source);
    }

    ~Logger();
private:
    LogSink &m_sink;
};

}    // namespace Log

inline Log::Logger logger;