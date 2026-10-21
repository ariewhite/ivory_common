#include <cstdio>
#include <memory>
#include <filesystem>
#include <fstream>

#include <ivory/core/logger.h>
#include <ivory/net/init.h>

#include <curl/easy.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <stdexcept>

constexpr const char *piston = "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json";
constexpr const char *
piston_sha256 = "1e9f0052c6f385576f0effac072f2b32278346ce647d681e396dcf7b6f7d738a";
std::string to_hex( const unsigned char* ptr, unsigned len);
struct data
{
     std::ofstream out;
     size_t size = 0;
     EVP_MD_CTX *md = nullptr;
};

size_t write_callback( char *ptr, size_t size, size_t nmemb, data *userdata );
int progress_callback( void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow );

int main( int argc, char **argv )
{
     namespace fs = std::filesystem;
     logger::init( "Init: {}", argv[0] );
     ivo::net::easy e;

     CURL *curl;
     CURLcode res;
     data dt;

     const fs::path tmp = fs::path( "/home/white/code/ivory/test" ).concat( ".part" );

     std::unique_ptr<EVP_MD_CTX, decltype( &EVP_MD_CTX_free )> md( nullptr, &EVP_MD_CTX_free );
     md.reset( EVP_MD_CTX_new() );
     if( !md || EVP_DigestInit_ex( md.get(), EVP_sha256(), nullptr ) != 1 )
          throw std::runtime_error( "sha256 init failed" );
     dt.md = md.get();

     curl = curl_easy_init();
     if ( curl )
     {
          char err[CURL_ERROR_SIZE] = {};
          dt.out.open( tmp, std::ios::binary );
          curl_easy_setopt( curl, CURLOPT_URL, piston );
          curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &write_callback );
          curl_easy_setopt( curl, CURLOPT_WRITEDATA, &dt );
          curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, err );
          curl_easy_setopt( curl, CURLOPT_NOPROGRESS, 0L );
          curl_easy_setopt( curl, CURLOPT_XFERINFODATA, &dt );
          curl_easy_setopt( curl, CURLOPT_XFERINFOFUNCTION, &progress_callback );

          res = curl_easy_perform( curl );

          curl_easy_cleanup( curl );

          dt.out.close();

          if ( res != CURLE_OK )
          {
               logger::warning( "some errors: {}", err[0] ? err : curl_easy_strerror( res ) );
               return 0;
          }

          if( md )
          {
               unsigned char dig[EVP_MAX_MD_SIZE];
               unsigned len = 0;
               EVP_DigestFinal_ex( md.get(), dig, &len );
               if( to_hex( dig, len) != piston_sha256 )
                    throw std::runtime_error( "sha256 mismtach" );
          }
          logger::info( "total accepted {} bytes", dt.size );
     }

     return 0;
}

size_t write_callback( char *ptr, size_t size, size_t nmemb, data *userdata )
{
     data *c = static_cast<data *>( userdata );
     size_t total_size = size * nmemb;
     c->out.write( ptr, total_size );
     c->size += total_size;
     if ( !c->out )
     {
          logger::error( "error occured" );
          return 0;
     }
     // sum sha256
     if( c->md )
          EVP_DigestUpdate( c->md, ptr, total_size );

     return total_size;
}

int progress_callback( void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow )
{
     logger::debug( "{}/{}", dlnow, dltotal );
     return 0;
}

std::string to_hex( const unsigned char* ptr, unsigned len)
{
     static const char k[] = "0123456789abcdef";
     std::string s;
     for ( unsigned i = 0 ; i < len; ++i )
     {
          s.push_back( k[ptr[i] >> 4]);
          s.push_back( k[ptr[i] & 0xf]);
     }
     logger::debug("sha256: {}", s);
     return s;
}