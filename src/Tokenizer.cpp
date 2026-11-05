#include "Tokenizer.h"

#include <cctype>

namespace Tokenizer {

std::string lower(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

std::string stem(const std::string& name) {
    std::string result = lower(name);
    std::size_t dot = result.find_last_of('.');
    if (dot != std::string::npos && dot > 0) {
        result.erase(dot);
    }
    return result;
}

std::string prefix(const std::string& stem) {
    std::string letters;
    for (char c : stem) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            letters += c;
            if (letters.size() == 3) break;
        }
    }
    return letters.size() == 3 ? letters : std::string();
}

std::vector<std::string> tokens(const std::string& stem) {
    std::vector<std::string> result;
    std::string current;
    for (char c : lower(stem)) {
        if (c == ' ' || c == '_' || c == '-' || c == '.') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) result.push_back(current);
    return result;
}

}
