#include "nlohmann/json_fwd.hpp"
#include <common/logger.h>
#include <ivory/version.h>

int main( int argc, char** argv )
{
     logger::init( "Init" );

     nlohmann::json json;
     auto x = ivory::parse( "hello", json );
     if( x.first != 0 )
     {
          logger::error( "{}", x.second );
     }
     
     return 0;
}