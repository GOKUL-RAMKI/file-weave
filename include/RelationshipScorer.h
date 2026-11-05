#pragma once

#include "FileNode.h"
#include "RelationshipEdge.h"

namespace Scoring {

constexpr double weightJaccard = 0.40;
constexpr double weightEdit = 0.30;
constexpr double weightPath = 0.20;
constexpr double weightMetadata = 0.10;
constexpr double defaultThreshold = 0.50;

}

RelationshipEdge scorePair(const FileNode& a, const FileNode& b);
