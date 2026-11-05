#pragma once

#include <string>
#include <vector>

#include "FileNode.h"

double jaccard(const std::vector<std::string>& a, const std::vector<std::string>& b);
double editSimilarity(const std::string& a, const std::string& b);
double pathProximity(const std::string& dirA, const std::string& dirB);
double metadataSimilarity(const FileNode& a, const FileNode& b);
