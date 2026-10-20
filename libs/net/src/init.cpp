/// @brief Curl wrapper

#include <ivory/net/init.h>

#include <stdexcept>

#include <curl/curl.h>

namespace ivo
{

namespace net
{

void curl_wrapper::ensure()
{
     static curl_wrapper instance;
     (void)instance;
}

curl_wrapper::curl_wrapper()
{
     if ( curl_global_init( CURL_GLOBAL_ALL ) != CURLE_OK )
     {
          throw std::runtime_error( "curl_global_init failed" );
     }
}

curl_wrapper::~curl_wrapper() { curl_global_cleanup(); }

/// -- easy

easy::easy()
{
     curl_wrapper::ensure();
}

CURL* easy::get() const noexcept
{
     return cu_.get();
}


} // namespace net

} // namespace ivo