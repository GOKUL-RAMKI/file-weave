#pragma once

#include <vector>

#include "RelationshipEdge.h"

using namespace std;

struct Point {
    double x = 0;
    double y = 0;
};

vector<Point> computeLayout(int nodeCount, const vector<RelationshipEdge>& forest, int iterations = 300);
