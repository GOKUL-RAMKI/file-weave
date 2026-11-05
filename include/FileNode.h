#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct FileNode {
    int id = -1;
    std::string name;
    std::string stem;
    std::string extension;
    std::string path;
    std::string dir;
    std::uintmax_t size = 0;
    bool hasSize = false;
    std::int64_t modified = 0;
    bool hasTime = false;
    std::vector<std::string> tokens;
};
