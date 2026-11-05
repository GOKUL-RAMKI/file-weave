#pragma once

#include <string>
#include <vector>

#include "FileNode.h"

struct ScanResult {
    std::vector<FileNode> files;
    int skipped = 0;
};

ScanResult scanDirectory(const std::string& root);
