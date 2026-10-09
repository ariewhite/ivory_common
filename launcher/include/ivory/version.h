#pragma once

#include <string>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

namespace ivory
{

std::pair< int, std::string > parse( const std::string_view path, nlohmann::json& ref );

} // ivory