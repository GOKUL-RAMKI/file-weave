#include "InvertedIndex.h"

#include <algorithm>
#include <cstdint>
#include <unordered_set>

void InvertedIndex::add(const std::string& token, int file) {
    if (!token.empty()) map[token].push_back(file);
}

std::vector<std::pair<int, int>> InvertedIndex::pairs(std::size_t maxPostings) const {
    std::vector<std::pair<int, int>> result;
    std::unordered_set<std::uint64_t> seen;

    for (const auto& entry : map) {
        const std::vector<int>& files = entry.second;
        if (files.size() > maxPostings) continue;

        for (std::size_t i = 0; i < files.size(); ++i) {
            for (std::size_t j = i + 1; j < files.size(); ++j) {
                int a = files[i];
                int b = files[j];
                if (a > b) std::swap(a, b);
                std::uint64_t key = (static_cast<std::uint64_t>(a) << 32) | static_cast<std::uint32_t>(b);
                if (seen.insert(key).second) result.push_back({a, b});
            }
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}
