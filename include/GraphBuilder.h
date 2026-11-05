#pragma once

#include <vector>

#include "FileNode.h"
#include "RelationshipEdge.h"

struct Graph {
    std::vector<FileNode> nodes;
    std::vector<RelationshipEdge> edges;
};

Graph buildGraph(std::vector<FileNode> nodes, double threshold);
