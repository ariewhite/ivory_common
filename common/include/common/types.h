#pragma once

#include <memory>
#include <string>

namespace ivo
{

typedef std::pair<int, std::string> err;
typedef std::shared_ptr<err> err_ptr;

} // namespace ivo