#pragma once

#include <filesystem>

#include "FileNode.h"

using namespace std;

FileNode collectMetadata(const filesystem::path& path);
