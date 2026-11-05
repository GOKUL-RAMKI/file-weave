#include <algorithm>
#include <cmath>
#include <vector>

#include "GraphLayout.h"
#include "check.h"

namespace {

constexpr double maxWidth = 1500.0;
constexpr double maxHeight = 950.0;

std::vector<RelationshipEdge> chain(int count) {
    std::vector<RelationshipEdge> edges;
    for (int i = 1; i < count; ++i) {
        RelationshipEdge edge;
        edge.a = i - 1;
        edge.b = i;
        edges.push_back(edge);
    }
    return edges;
}

void checkBounded(int count) {
    std::vector<Point> points = computeLayout(count, chain(count));
    check(static_cast<int>(points.size()) == count, "layout returns one point per node");

    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    for (const Point& point : points) {
        check(std::isfinite(point.x) && std::isfinite(point.y), "layout coordinates are finite");
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    check(maxX - minX <= maxWidth, "layout width stays bounded");
    check(maxY - minY <= maxHeight, "layout height stays bounded");
}

}

int main() {
    check(computeLayout(0, {}).empty(), "empty layout");
    check(computeLayout(1, {}).size() == 1, "single node layout");

    checkBounded(2);
    checkBounded(3);
    checkBounded(10);
    checkBounded(100);
    checkBounded(300);
    checkBounded(610);

    std::vector<Point> small = computeLayout(6, chain(6));
    double minX = small[0].x, maxX = small[0].x, minY = small[0].y, maxY = small[0].y;
    for (const Point& point : small) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }
    check(maxX - minX > 50.0 || maxY - minY > 50.0, "small layouts are not collapsed");

    RelationshipEdge invalid;
    invalid.a = 0;
    invalid.b = 5;
    std::vector<Point> guard = computeLayout(3, { invalid });
    check(guard.size() == 3, "invalid forest edge is ignored");

    return 0;
}
