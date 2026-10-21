#pragma once

#include <filesystem>

#include <ivory/core/types.h>

namespace ivo
{

namespace paths
{

/// @brief Full path to executable
result<std::filesystem::path> executable_path();

/// @brief Full path to dir, where located executable
result<std::filesystem::path> executable_dir();

/// @brief Return true, if portable version
/// @note rn app work always in portable mode
bool is_portable();

} // namespace paths

} // namespace ivo