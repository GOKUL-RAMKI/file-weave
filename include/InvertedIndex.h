#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class InvertedIndex {
public:
    void add(const string& token, int file);
    vector<pair<int, int>> pairs(size_t maxPostings) const;

private:
    unordered_map<string, vector<int>> map;
};
