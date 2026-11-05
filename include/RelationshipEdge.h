#pragma once

struct RelationshipEdge {
    int a = -1;
    int b = -1;
    double jaccard = 0;
    double edit = 0;
    double path = 0;
    double metadata = 0;
    double score = 0;
    double cost = 1;
};
