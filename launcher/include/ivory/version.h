#pragma once

#include <string>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

#include <common/types.h>

namespace ivo
{

ivo::err_ptr parse( const std::string path, nlohmann::json& ref );

} // namespace ivo