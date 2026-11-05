#pragma once

#include <vector>

#include "RelationshipEdge.h"

struct Point {
    double x = 0;
    double y = 0;
};

std::vector<Point> computeLayout(int nodeCount, const std::vector<RelationshipEdge>& forest, int iterations = 300);
