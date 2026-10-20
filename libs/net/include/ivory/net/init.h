/// @brief Curl wrapper
#pragma once

#include <memory>

#include <curl/curl.h>
#include <curl/easy.h>

namespace ivo
{

namespace net
{

struct easy_deleter
{
     void operator()( CURL *h ) const { curl_easy_cleanup( h ); }
};
using easy_ptr = std::unique_ptr<CURL, easy_deleter>;

class curl_wrapper
{
   public:
     static void ensure();

     curl_wrapper( const curl_wrapper & ) = delete;
     curl_wrapper &operator=( const curl_wrapper & ) = delete;

   private:
     curl_wrapper();
     ~curl_wrapper();
};

class easy
{
   public:
     easy();

     CURL *get() const noexcept;

   private:
     easy_ptr cu_;
};

} // namespace net

} // namespace ivo