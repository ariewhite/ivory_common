#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <curl/curl.h>
#include <curl/easy.h>
#include <openssl/evp.h>

#include <ivory/net/init.h>

#include <ivory/tasks/download_task.h>

namespace ivo
{

namespace
{

struct dt_ctx
{
     std::ofstream out;
     EVP_MD_CTX *md = nullptr;
     std::function<bool( uint64_t, uint64_t )> progress;
};

size_t write_cb( char *p, size_t sz, size_t n, void *userdata )
{
     auto *ctx = static_cast<dt_ctx *>( userdata );
     const size_t len = sz * n;
     ctx->out.write( p, static_cast<std::streamsize>( len ) );
     if ( !ctx->out )
          return 0; // @todo: add some error handler or signal
     if ( ctx->md )
          EVP_DigestUpdate( ctx->md, p, len );
     return len;
}

int xferinfo_cb( void *ud, curl_off_t total, curl_off_t now, curl_off_t, curl_off_t )
{
     auto *ctx = static_cast<dt_ctx *>( ud );
     return ctx->progress( static_cast<uint64_t>( now ), static_cast<uint64_t>( total ) ) ? 0 : 1;
}

std::string to_hex( const unsigned char *d, unsigned len )
{
     static const char k[] = "0123456789abcdef";
     std::string s;
     for ( unsigned i = 0; i < len; ++i )
     {
          s.push_back( k[d[i] >> 4] );
          s.push_back( k[d[i] & 0xF] );
     }
     return s;
}

} // namespace

namespace fs = std::filesystem;

download_task::download_task( std::string url, std::filesystem::path dest, std::string sha256 )
    : url_( url ), dest_( dest ), sha256_( sha256 )
{
}

std::string download_task::name() const { return "download " + url_; }

void download_task::on_start()
{
     if ( dest_.has_parent_path() )
          fs::create_directories( dest_.parent_path() );

     const fs::path tmp = fs::path( dest_ ).concat( ".part" );

     dt_ctx ctx;
     ctx.progress = [this]( uint64_t done, uint64_t total )
     {
          set_progress( done, total );
          return !is_done();
     };

     ctx.out.open( tmp, std::ios::binary );
     if ( !ctx.out )
          throw std::runtime_error( "can not open " + tmp.string() );

     std::unique_ptr<EVP_MD_CTX, decltype( &EVP_MD_CTX_free )> md( nullptr, &EVP_MD_CTX_free );
     // if sha256 present, should to check it
     if ( !sha256_.empty() )
     {
          md.reset( EVP_MD_CTX_new() );
          if ( !md || EVP_DigestInit_ex( md.get(), EVP_sha256(), nullptr ) != 1 )
               throw std::runtime_error( "sha256 init failed" );
          ctx.md = md.get();
     }

     std::error_code ec;
     char err[CURL_ERROR_SIZE] = {};

     try
     {
          // @todo: i started write wrapper for pure curl, but rn it work in pure C style
          // should replace in future
          net::easy e;

          CURL *curl;
          CURLcode res;
          curl = curl_easy_init();

          if ( curl )
          {
               curl_easy_setopt( curl, CURLOPT_URL, url_.c_str() );
               curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &write_cb );
               curl_easy_setopt( curl, CURLOPT_WRITEDATA, &ctx );
               curl_easy_setopt( curl, CURLOPT_TIMEOUT, 15L );
               curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, err );
               curl_easy_setopt( curl, CURLOPT_NOPROGRESS, 0L );
               curl_easy_setopt( curl, CURLOPT_XFERINFODATA, &ctx );
               curl_easy_setopt( curl, CURLOPT_XFERINFOFUNCTION, &xferinfo_cb );

               res = curl_easy_perform( curl );

               // aborted
               if ( res == CURLE_ABORTED_BY_CALLBACK )
               {
                    // remove temp file
                    fs::remove( tmp, ec );
                    return;
               }

               // curl issues
               if ( res != CURLE_OK )
               {
                    throw std::runtime_error( err[0] ? err : curl_easy_strerror( res ) );
               }

               // check hash sum
               if ( md )
               {
                    unsigned char digest[EVP_MAX_MD_SIZE];
                    unsigned len = 0;
                    EVP_DigestFinal_ex(md.get(), digest, &len);
                    if ( to_hex( digest, len ) != sha256_ )
                    {
                         throw std::runtime_error( "sha256 mismatch, file: " + dest_.filename().string() );
                    }
               }

               // cancelled
               if( is_done() )
               {
                    fs::remove( tmp, ec );
                    return;
               }

               fs::rename( tmp, dest_ );
          }
     }
     catch ( ... )
     {
          ctx.out.close();
          fs::remove( tmp, ec );
          throw;
     }

     succeed();
}
void on_cancel() {}

} // namespace ivo
