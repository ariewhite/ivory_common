#include <cstdio>
#include <fstream>
#include <filesystem>
#include <curl/curl.h>
#include <ivory/core/logger.h>
#include <ivory/net/init.h>

#include <curl/easy.h>

constexpr const char* piston = "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json";
constexpr const char* piston_sha256 = "1e9f0052c6f385576f0effac072f2b32278346ce647d681e396dcf7b6f7d738a";

struct data
{
     std::ofstream out;
     size_t size = 0;
};

size_t write_callback(char *ptr, size_t size, size_t nmemb, data *userdata);

int main( int argc, char** argv )
{
     namespace fs = std::filesystem;
     logger::init( "Init: {}", argv[0] );
     ivo::net::easy e;

     CURL* curl;
     CURLcode res;
     data dt;

     const fs::path tmp = fs::path( "/home/white/code/ivory/test" ).concat(".part");
     
     curl = curl_easy_init();
     if( curl )
     {
          char err[CURL_ERROR_SIZE];
          dt.out.open( tmp, std::ios::binary );
          curl_easy_setopt( curl, CURLOPT_URL, piston );
          curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &write_callback );
          curl_easy_setopt( curl, CURLOPT_WRITEDATA, &dt );
          curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, err);

          res = curl_easy_perform( curl );

          curl_easy_cleanup( curl );

          dt.out.close();

          if ( res != CURLE_OK )
          {
               logger::warning( "some errors: {}", err[0] ? err : curl_easy_strerror( res ) );
               return 0;
          }
          logger::info( "total accepted {} bytes", dt.size );
     }

     return 0;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, data *userdata)
{
     data* c = static_cast<data*>( userdata );
     size_t total_size = size * nmemb;
     c->out.write( ptr, total_size );
     c->size += total_size;
     if( !c->out )
     {
          logger::error( "error occured" );
     }
     logger::debug( "writed: {} bytes", userdata->size );
     return total_size;
}