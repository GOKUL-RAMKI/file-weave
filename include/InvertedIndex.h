#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class InvertedIndex {
public:
    void add(const std::string& token, int file);
    std::vector<std::pair<int, int>> pairs(std::size_t maxPostings) const;

private:
    std::unordered_map<std::string, std::vector<int>> map;
};
