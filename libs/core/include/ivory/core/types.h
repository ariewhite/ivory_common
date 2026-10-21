#pragma once

#include <memory>
#include <string>
#include <expected>

namespace ivo
{

typedef std::pair<int, std::string> err;
typedef std::shared_ptr<err> err_ptr;

template < typename T >
using result = std::expected< T, std::error_code >;

} // namespace ivo