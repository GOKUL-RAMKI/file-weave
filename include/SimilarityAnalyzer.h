#pragma once

#include <string>
#include <vector>

#include "FileNode.h"

using namespace std;

double jaccard(const vector<string>& a, const vector<string>& b);
double editSimilarity(const string& a, const string& b);
double pathProximity(const string& dirA, const string& dirB);
double metadataSimilarity(const FileNode& a, const FileNode& b);
