#pragma once

#include <string>
#include <vector>

#include "FileNode.h"

using namespace std;

struct ScanResult {
    vector<FileNode> files;
    int skipped = 0;
};

ScanResult scanDirectory(const string& root);
