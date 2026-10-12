#include "ivory/version.h"

#include <exception>
#include <fstream>

#include <ivory/core/logger.h>
#include <ivory/core/types.h>

namespace ivo
{
using nlohmann::json;

ivo::err_ptr parse( const std::string path, nlohmann::json& ref )
{
     try
     {
          std::ifstream f{ path };
          if( !f ) return std::make_shared<err>( -1, "cannot open file" );
          ref = json::parse( f );
     }
     catch( const std::exception& e )
     {
          logger::error( "exception while parsing json: {}, path: {}", e.what(), path );
          return std::make_shared< ivo::err >( -1, "exception occured check logs" );
     }

     return nullptr;
}

} // ivory