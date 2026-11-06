#pragma once

#include <vector>

#include "FileNode.h"
#include "RelationshipEdge.h"

using namespace std;

struct Graph {
    vector<FileNode> nodes;
    vector<RelationshipEdge> edges;
};

Graph buildGraph(vector<FileNode> nodes, double threshold);
