#pragma once

#include <vector>

using namespace std;

class DisjointSet {
public:
    explicit DisjointSet(int count);
    int find(int x);
    bool unite(int a, int b);

private:
    vector<int> parent;
    vector<int> rank;
};
