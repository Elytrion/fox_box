#pragma once
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace logger {

    enum class Level {
        Info,
        Warn,
        Error
    };

    inline const char* level_name(Level level) noexcept
    {
        switch (level) {
        case Level::Info:  return "INFO";
        case Level::Warn:  return "WARN";
        case Level::Error: return "ERROR";
        }

        return "UNKNOWN";
    }

    inline std::mutex& output_mutex()
    {
        static std::mutex mutex;
        return mutex;
    }

    template <typename T>
    std::string to_string(T&& value)
    {
        std::ostringstream stream;
        stream << std::forward<T>(value);
        return stream.str();
    }

    inline std::string format_message(std::string_view format)
    {
        return std::string(format);
    }

    template <typename... Args>
    std::string format_message(std::string_view format, Args&&... args)
    {
        const std::array<std::string, sizeof...(Args)> values{
            to_string(std::forward<Args>(args))...
        };

        std::string result;
        result.reserve(format.size() + values.size() * 8);

        std::size_t argument_index = 0;

        for (std::size_t i = 0; i < format.size(); ++i) {
            // Escaped opening brace: {{
            if (format[i] == '{' && i + 1 < format.size()) {
                if (format[i + 1] == '{') {
                    result.push_back('{');
                    ++i;
                    continue;
                }

                // Formatting placeholder: {}
                if (format[i + 1] == '}') {
                    if (argument_index < values.size()) {
                        result += values[argument_index++];
                    }
                    else {
                        // Leave unmatched placeholders visible.
                        result += "{}";
                    }

                    ++i;
                    continue;
                }
            }

            // Escaped closing brace: }}
            if (format[i] == '}' &&
                i + 1 < format.size() &&
                format[i + 1] == '}') {
                result.push_back('}');
                ++i;
                continue;
            }

            result.push_back(format[i]);
        }

        // Do not silently discard extra arguments.
        if (argument_index < values.size()) {
            result += " [extra:";

            while (argument_index < values.size()) {
                result.push_back(' ');
                result += values[argument_index++];
            }

            result.push_back(']');
        }

        return result;
    }

    inline std::tm local_time(std::time_t time)
    {
        std::tm result{};

#if defined(_WIN32)
        localtime_s(&result, &time);
#else
        localtime_r(&time, &result);
#endif

        return result;
    }

    template <typename... Args>
    void log(
        Level level,
        const char* file,
        int line,
        std::string_view format,
        Args&&... args
    ) noexcept
    {
        try {
            const std::string message =
                format_message(format, std::forward<Args>(args)...);

            const auto now = std::chrono::system_clock::now();
            const std::time_t now_time =
                std::chrono::system_clock::to_time_t(now);

            const std::tm time = local_time(now_time);

            // Prevent multiple threads from mixing their output.
            std::lock_guard<std::mutex> lock(output_mutex());

            std::ostream& output =
                level == Level::Error ? std::cerr : std::clog;

            output
                << '[' << std::put_time(&time, "%Y-%m-%d %H:%M:%S") << "] "
                << '[' << level_name(level) << "] "
                << message
                << " (" << file << ':' << line << ")\n";
        }
        catch (...) {
            // Logging should not terminate the application.
        }
    }

} // namespace logger

#define LOG_INFO(...)                                                   \
    ::logger::log(                                                  \
        ::logger::Level::Info, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_WARN(...)                                                   \
    ::logger::log(                                                  \
        ::logger::Level::Warn, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_ERROR(...)                                                  \
    ::logger::log(                                                  \
        ::logger::Level::Error, __FILE__, __LINE__, __VA_ARGS__)
