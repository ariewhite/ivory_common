#pragma once

#include <string>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

#include <ivory/core/types.h>

namespace ivo
{

ivo::err_ptr parse( const std::string path, nlohmann::json& ref );

} // namespace ivo