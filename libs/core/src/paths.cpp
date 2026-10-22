#include <expected>
#include <filesystem>
#include <system_error>

#include <ivory/core/paths.h>

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

constexpr const char *portable_marker = "portable.txt";

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
#if !defined( _WIN32 )
[[maybe_unused]]
result<fs::path> home_dir()
{
     if ( auto h = get_env( "HOME" ) )
     {
          return fs::path( *h );
     }
     return std::unexpected( std::make_error_code( std::errc::no_such_file_or_directory ) );
}
#endif

/// return executable path for linux, windows, macos(not tested)
result<fs::path> raw_executable_path()
{
#if defined( _WIN32 )
     std::wstring buf;
     for ( std::size_t cap = MAX_PATH;; cap *= 2 )
     {
          DWORD n = 0;
          DWORD err = 0;
          buf.resize_and_overwrite( cap,
                                    [&]( wchar_t *p, std::size_t c )
                                    {
                                         n = ::GetModuleFileNameW( nullptr, p, static_cast<DWORD>( c ) );
                                         if ( 0 == n )
                                         {
                                              err = ::GetLastError();
                                         }
                                         return static_cast<std::size_t>( n );
                                    } );
          if ( 0 == n )
          {
               return std::unexpected( std::error_code( static_cast<int>( err ), std::system_category() ) );
          }
          if ( n < cap )
          {
               return fs::path( buf );
          }
     }
#elif defined( __APPLE__ ) // writed by ai, not tested
     uint32_t size = 0;
     _NSGetExecutablePath( nullptr, &size );
     std::string buf;
     bool ok = false;
     buf.resize_and_overwrite( size,
                               [&]( char *p, std::size_t )
                               {
                                    ok = _NSGetExecutablePath( p, &size ) == 0;
                                    return ok ? std::strlen( p ) : std::size_t{ 0 };
                               } );
     if ( !ok )
          return fail( std::errc::bad_address );
     return fs::path( buf );
#else
     std::string buf;
     for ( std::size_t cap = 256;; cap *= 2 )
     {
          ssize_t n = 0;
          int err = 0;
          buf.resize_and_overwrite( cap,
                                    [&]( char *p, std::size_t c )
                                    {
                                         n = ::readlink( "/proc/self/exe", p, c );
                                         if ( n < 0 )
                                         {
                                              err = errno;
                                              return size_t{ 0 };
                                         }
                                         return static_cast<std::size_t>( n );
                                    } );
          if ( n < 0 )
          {
               return std::unexpected( std::error_code( err, std::system_category() ) );
          }
          if ( static_cast<std::size_t>( n ) < cap )
          {
               return fs::path( buf );
          }
     }
#endif
}


} // namespace

result<fs::path> executable_path()
{
     static const result<fs::path> cached = raw_executable_path().and_then( canonicalize );
     return cached;
}

result<std::filesystem::path> executable_dir()
{
     return executable_path().transform( []( const fs::path &p ) { return p.parent_path(); } );
}

bool is_portable() { return true; }

} // namespace paths

} // namespace ivo