/// @brief Download from uri / path task

#pragma once

#include <filesystem>
#include <string>

#include <ivory/tasks/task_base.h>

namespace ivo
{

class download_task : public ivo::base_task
{
   public:
     download_task( const std::string url, const std::filesystem::path dest, const std::string sha256 );

     std::string name() const override;

   protected:
     void on_start() override;
     void on_cancel() override;

   private:
     std::string url_;
     std::filesystem::path dest_;
     /// @note hex, lowercase, empty - dont verify
     std::string sha256_;
};

} // namespace ivo
