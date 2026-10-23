#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include <ivory/net/init.h>
#include <ivory/tasks/download_task.h>

namespace fs = std::filesystem;

namespace
{

// sha1( "abc" )
const std::string k_abc_sha256 = "f09921bbfd54646ce1ddbe92caf6a8dce4502a9a";

std::string file_url( const fs::path &p )
{
     const auto s = p.generic_string();
     return s.front() == '/' ? "file://" + s : "file:///" + s; // posix / windows
}

std::string read_all( const fs::path &p )
{
     std::ifstream in( p, std::ios::binary );
     return { std::istreambuf_iterator<char>( in ), std::istreambuf_iterator<char>() };
}

} // namespace

class DownloadTaskTest : public ::testing::Test
{
   protected:
     void SetUp() override
     {
          // @todo: если net::init требует явной инициализации (curl_global_init) - вызвать здесь
          const auto *info = ::testing::UnitTest::GetInstance()->current_test_info();
          dir_ = fs::temp_directory_path() / ( std::string( "ivory_dl_" ) + info->name() );
          fs::remove_all( dir_ );
          fs::create_directories( dir_ );

          src_ = dir_ / "src.bin";
          std::ofstream( src_, std::ios::binary ) << "abc";
          dest_ = dir_ / "out" / "dst.bin"; // заодно проверяем create_directories
     }

     void TearDown() override
     {
          std::error_code ec;
          fs::remove_all( dir_, ec );
     }

     fs::path dir_, src_, dest_;
};

TEST_F( DownloadTaskTest, SucceedsWithCorrectHash )
{
     ivo::download_task t( file_url( src_ ), dest_, k_abc_sha256 );
     t.start();

     EXPECT_EQ( t.state(), ivo::task_state::succeeded ) << t.error().msg;
     EXPECT_TRUE( fs::exists( dest_ ) );
     EXPECT_EQ( read_all( dest_ ), "abc" );
     EXPECT_FALSE( fs::exists( fs::path( dest_ ).concat( ".part" ) ) );
}

TEST_F( DownloadTaskTest, SucceedsWithoutHash )
{
     ivo::download_task t( file_url( src_ ), dest_, "" );
     t.start();

     EXPECT_EQ( t.state(), ivo::task_state::succeeded ) << t.error().msg;
     EXPECT_EQ( read_all( dest_ ), "abc" );
}

TEST_F( DownloadTaskTest, FailsOnHashMismatch )
{
     ivo::download_task t( file_url( src_ ), dest_, std::string( 64, '0' ) );
     t.start();

     EXPECT_EQ( t.state(), ivo::task_state::failed );
     EXPECT_NE( t.error().msg.find( "sha1 mismatch" ), std::string::npos ) << t.error().msg;
     EXPECT_FALSE( fs::exists( dest_ ) );
     EXPECT_FALSE( fs::exists( fs::path( dest_ ).concat( ".part" ) ) );
}

TEST_F( DownloadTaskTest, FailsOnMissingSource )
{
     ivo::download_task t( file_url( dir_ / "nope.bin" ), dest_, "" );
     t.start();

     EXPECT_EQ( t.state(), ivo::task_state::failed );
     EXPECT_FALSE( t.error().msg.empty() );
     EXPECT_FALSE( fs::exists( dest_ ) );
}

TEST_F( DownloadTaskTest, ReportsProgress )
{
     ivo::download_task t( file_url( src_ ), dest_, "" );
     t.start();

     const auto p = t.progress();
     EXPECT_EQ( p.done, 3u );
}

// Живой тест, запускать вручную: --gtest_also_run_disabled_tests
TEST_F( DownloadTaskTest, DISABLED_DownloadManifestLive )
{
     ivo::download_task t( "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json",
                           dest_, "" ); // хеш не фиксируем: манифест меняется
     t.start();

     EXPECT_EQ( t.state(), ivo::task_state::succeeded ) << t.error().msg;
     EXPECT_GT( fs::file_size( dest_ ), 0u );
}