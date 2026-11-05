#pragma once

#include <string>
#include <vector>

namespace Tokenizer {

std::string lower(const std::string& text);
std::string stem(const std::string& name);
std::string prefix(const std::string& stem);
std::vector<std::string> tokens(const std::string& stem);

}
