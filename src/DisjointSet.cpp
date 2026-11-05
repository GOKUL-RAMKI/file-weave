#include "DisjointSet.h"

DisjointSet::DisjointSet(int count) : parent(count), rank(count, 0) {
    for (int i = 0; i < count; ++i) parent[i] = i;
}

int DisjointSet::find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

bool DisjointSet::unite(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);
    if (rootA == rootB) return false;

    if (rank[rootA] < rank[rootB]) {
        parent[rootA] = rootB;
    } else if (rank[rootA] > rank[rootB]) {
        parent[rootB] = rootA;
    } else {
        parent[rootB] = rootA;
        ++rank[rootA];
    }
    return true;
}
