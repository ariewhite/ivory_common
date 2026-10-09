#include <ivory/version.h>

namespace ivory
{

std::pair< int, std::string > parse( const std::string_view path, nlohmann::json& ref )
{
     return { -1, std::string( "Error with: " ) + std::string( path ) };
}

} // ivory