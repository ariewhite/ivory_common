#include <curl/curl.h>
#include <curl/easy.h>
#include <ivory/core/logger.h>
#include <ivory/core/paths.h>
#include <ivory/net/init.h>
#include <ivory/tasks/download_task.h>
#include <openssl/evp.h>

#include <cstdio>
#include <filesystem>

int main(int argc, char **argv)
{
     logger::info("exe path: \"{}\"\ndir path: \"{}\"",
                  ivo::paths::executable_path().value().string(),
                  ivo::paths::executable_dir().value().string());
     return 1;
}