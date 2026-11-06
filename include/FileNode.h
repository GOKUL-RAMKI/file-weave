#pragma once

#include <cstdint>
#include <string>
#include <vector>

using namespace std;

struct FileNode {
    int id = -1;
    string name;
    string stem;
    string extension;
    string path;
    string dir;
    uintmax_t size = 0;
    bool hasSize = false;
    int64_t modified = 0;
    bool hasTime = false;
    vector<string> tokens;
};
