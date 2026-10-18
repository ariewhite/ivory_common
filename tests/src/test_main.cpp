#include "nlohmann/json.hpp"
#include <ivory/core/logger.h>
#include <ivory/version.h>


int main( int argc, char** argv )
{
     logger::init( "Init" );

     nlohmann::json json;
     auto x = ivo::parse( "test.json", json );
     if( x )
     {
          logger::error( "{}", x->second );
     }

     return 0;
}