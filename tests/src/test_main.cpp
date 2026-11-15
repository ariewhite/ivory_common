#include <cstdio>
#include <filesystem>
#include <fstream>

#include <ivory/core/logger.h>
#include <ivory/net/init.h>
#include <ivory/core/paths.h>
#include <ivory/tasks/download_task.h>

#include <curl/easy.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <stdexcept>

int main( int argc, char **argv )
{
     logger::info( "exe path: \"{}\"\ndir path: \"{}\"", ivo::paths::executable_path().value().string(), ivo::paths::executable_dir().value().string() );
     return 1;


     return 0;
}