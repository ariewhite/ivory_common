#include <expected>
#include <filesystem>
#include <ivory/core/paths.h>
#include <system_error>

/// @note APPLE support not tested
#if defined( _WIN32 )
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <windows.h>
#elif defined( __APPLE__ )
#include <cstring>
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

namespace ivo
{

namespace paths
{

namespace fs = std::filesystem;

namespace
{

constexpr const char *portable_marker = "ivoportable.txt";

/// @brief get env variable
std::optional<std::string> get_env( const char *env )
{
     if ( const char *v = std::getenv( env ); v && *v )
          return std::string( v );
     return {};
}

result<fs::path> canonicalize( const fs::path &p )
{
     std::error_code ec;
     auto r = fs::weakly_canonical( p, ec );
     if ( ec )
          return std::unexpected( ec );
     return r;
}

/// only for unix
#if !defined(_WIN32)
result<fs::path> home_dir()
{
     if ( auto h = get_env( "HOME" ) )
     {
          return fs::path( *h );
     }
     return std::unexpected( std::make_error_code( std::errc::no_such_file_or_directory ) );
}
#endif

} // namespace

result<fs::path> executable_path() {}

result<std::filesystem::path> executable_dir();

bool is_portable() { return true; }

} // namespace paths

} // namespace ivo