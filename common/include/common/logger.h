#pragma once

#include <cstdio>
#include <print>
#include <string_view>
#include <utility>

namespace clr
{
inline constexpr std::string_view black = "\033[30m";
inline constexpr std::string_view red = "\033[31m";
inline constexpr std::string_view green = "\033[32m";
inline constexpr std::string_view yellow = "\033[33m";
inline constexpr std::string_view blue = "\033[34m";
inline constexpr std::string_view magenta = "\033[35m";
inline constexpr std::string_view cyan = "\033[36m";
inline constexpr std::string_view white = "\033[37m";
inline constexpr std::string_view orange = "\033[38;5;208m";

inline constexpr std::string_view reset = "\033[0m";
inline constexpr std::string_view bold = "\033[1m";
inline constexpr std::string_view normal = "\033[22m";
} // namespace clr

namespace logger
{
namespace
{
template <typename... Args>
void write( std::FILE *out, std::string_view tag, std::string_view color, std::format_string<Args...> fmt,
            Args &&...args )
{
     std::print( out, "[{}{}{}{}] ", color, clr::bold, tag, clr::reset );
     std::println( out, fmt, std::forward<Args>( args )... );
}
} // namespace

template <typename... Args>
void error( std::format_string<Args...> fmt, Args &&...args )
{
     write( stderr, "ERROR", clr::red, fmt, std::forward<Args>( args )... );
}

template <typename... Args>
void init( std::format_string<Args...> fmt, Args &&...args )
{
     write( stdout, "INIT", clr::cyan, fmt, std::forward<Args>( args )... );
}

template <typename... Args>
void info( std::format_string<Args...> fmt, Args &&...args )
{
     write( stdout, "INFO", clr::yellow, fmt, std::forward<Args>( args )... );
}

template <typename... Args>
void warning( std::format_string<Args...> fmt, Args &&...args )
{
     write( stderr, "WARNING", clr::orange, fmt, std::forward<Args>( args )... );
}

template <typename... Args>
void debug( std::format_string<Args...> fmt, Args &&...args )
{
     write( stdout, "DEBUG", clr::white, fmt, std::forward<Args>( args )... );
}
} // namespace log
