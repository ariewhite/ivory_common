add_library(ivory_net STATIC
  src/init.cpp
)
add_library(ivory::net ALIAS ivory_net)

target_include_directories(ivory_net
  PUBLIC include
)
target_link_libraries(ivory_net PUBLIC
  ivory::core
  CURL::libcurl
  OpenSSL::SSL
)
