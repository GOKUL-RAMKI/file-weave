#include "MetadataCollector.h"

#include <chrono>
#include <system_error>

#include "Tokenizer.h"

namespace {

std::int64_t toEpochSeconds(std::filesystem::file_time_type time) {
    using namespace std::chrono;
    auto system = time_point_cast<system_clock::duration>(
        time - std::filesystem::file_time_type::clock::now() + system_clock::now());
    return duration_cast<seconds>(system.time_since_epoch()).count();
}

}

FileNode collectMetadata(const std::filesystem::path& path) {
    FileNode node;
    node.name = path.filename().u8string();
    node.path = path.u8string();
    node.dir = path.parent_path().u8string();
    node.stem = Tokenizer::stem(node.name);

    std::string extension = Tokenizer::lower(path.extension().u8string());
    if (!extension.empty() && extension.front() == '.') extension.erase(0, 1);
    node.extension = extension;

    std::error_code error;
    std::uintmax_t size = std::filesystem::file_size(path, error);
    if (!error) {
        node.size = size;
        node.hasSize = true;
    }

    std::filesystem::file_time_type time = std::filesystem::last_write_time(path, error);
    if (!error) {
        node.modified = toEpochSeconds(time);
        node.hasTime = true;
    }

    return node;
}
