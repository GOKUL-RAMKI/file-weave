#include "Kruskal.h"

#include <algorithm>

#include "DisjointSet.h"

std::vector<RelationshipEdge> kruskal(int nodeCount, std::vector<RelationshipEdge> edges) {
    std::sort(edges.begin(), edges.end(), [](const RelationshipEdge& left, const RelationshipEdge& right) {
        if (left.cost != right.cost) return left.cost < right.cost;
        if (left.a != right.a) return left.a < right.a;
        return left.b < right.b;
    });

    DisjointSet sets(nodeCount);
    std::vector<RelationshipEdge> forest;
    for (const RelationshipEdge& edge : edges) {
        if (sets.unite(edge.a, edge.b)) {
            forest.push_back(edge);
            if (static_cast<int>(forest.size()) == nodeCount - 1) break;
        }
    }

    return forest;
}
