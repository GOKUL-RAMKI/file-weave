#pragma once

#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int count);
    int find(int x);
    bool unite(int a, int b);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};
