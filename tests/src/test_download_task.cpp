#include <gtest/gtest.h>

#include <ivory/net/init.h>
#include <ivory/tasks/download_task.h>

TEST( DownloadTasks, DownloadManifest )
{
     constexpr std::string_view manifest_url { "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json" };
     // const std::string manifest_sha1 = "f09921bbfd54646ce1ddbe92caf6a8dce4502a9a";
     auto x = ivo::download_task( manifest_url, )
}