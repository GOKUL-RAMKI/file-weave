#pragma once

#include <filesystem>

#include "FileNode.h"

FileNode collectMetadata(const std::filesystem::path& path);
