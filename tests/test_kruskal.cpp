#include <cstddef>
#include <vector>

#include "Kruskal.h"
#include "check.h"

namespace {

RelationshipEdge makeEdge(int a, int b, double cost) {
    RelationshipEdge edge;
    edge.a = a;
    edge.b = b;
    edge.score = 1.0 - cost;
    edge.cost = cost;
    return edge;
}

double totalCost(const std::vector<RelationshipEdge>& forest) {
    double total = 0;
    for (const RelationshipEdge& edge : forest) total += edge.cost;
    return total;
}

}

int main() {
    check(kruskal(0, {}).empty(), "empty graph");
    check(kruskal(1, {}).empty(), "single vertex");

    std::vector<RelationshipEdge> single = kruskal(2, { makeEdge(0, 1, 0.5) });
    check(single.size() == 1, "single edge selected");
    checkNear(totalCost(single), 0.5, "single edge cost");

    std::vector<RelationshipEdge> cycle = kruskal(3, {
        makeEdge(0, 1, 1),
        makeEdge(1, 2, 2),
        makeEdge(0, 2, 3)
    });
    check(cycle.size() == 2, "cycle broken");
    checkNear(totalCost(cycle), 3.0, "minimum two edges chosen");

    std::vector<RelationshipEdge> disconnected = kruskal(4, {
        makeEdge(0, 1, 0.5),
        makeEdge(2, 3, 0.7)
    });
    check(disconnected.size() == 2, "disconnected forest keeps both components");

    std::vector<RelationshipEdge> equal = kruskal(3, {
        makeEdge(0, 1, 1),
        makeEdge(0, 2, 1),
        makeEdge(1, 2, 1)
    });
    check(equal.size() == 2, "equal costs still avoid cycles");
    check(equal[0].a == 0 && equal[0].b == 1, "equal costs use stable order");
    check(equal[1].a == 0 && equal[1].b == 2, "second equal edge follows order");

    return 0;
}
