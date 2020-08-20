#pragma once

#include <vector>
#include <string>

namespace utils {

std::vector<std::string> split(const std::string& s, const char delimiter);
bool isSuffix(const std::string& s, const std::string& of);

} // namespace utils
