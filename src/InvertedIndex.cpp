#include "InvertedIndex.h"

#include <algorithm>
#include <cstdint>
#include <unordered_set>

using namespace std;

void InvertedIndex::add(const string& token, int file) {
    if (!token.empty()) map[token].push_back(file);
}

vector<pair<int, int>> InvertedIndex::pairs(size_t maxPostings) const {
    vector<pair<int, int>> result;
    unordered_set<uint64_t> seen;

    for (const auto& entry : map) {
        const vector<int>& files = entry.second;
        if (files.size() > maxPostings) continue;

        for (size_t i = 0; i < files.size(); ++i) {
            for (size_t j = i + 1; j < files.size(); ++j) {
                int a = files[i];
                int b = files[j];
                if (a > b) swap(a, b);
                uint64_t key = (static_cast<uint64_t>(a) << 32) | static_cast<uint32_t>(b);
                if (seen.insert(key).second) result.push_back({a, b});
            }
        }
    }

    sort(result.begin(), result.end());
    return result;
}
