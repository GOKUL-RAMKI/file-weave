#include "MetadataCollector.h"

#include <chrono>
#include <system_error>

#include "Tokenizer.h"

using namespace std;

namespace {

int64_t toEpochSeconds(filesystem::file_time_type time) {
    using namespace chrono;
    auto system = time_point_cast<system_clock::duration>(
        time - filesystem::file_time_type::clock::now() + system_clock::now());
    return duration_cast<seconds>(system.time_since_epoch()).count();
}

}

FileNode collectMetadata(const filesystem::path& path) {
    FileNode node;
    node.name = path.filename().u8string();
    node.path = path.u8string();
    node.dir = path.parent_path().u8string();
    node.stem = Tokenizer::stem(node.name);

    string extension = Tokenizer::lower(path.extension().u8string());
    if (!extension.empty() && extension.front() == '.') extension.erase(0, 1);
    node.extension = extension;

    error_code error;
    uintmax_t size = filesystem::file_size(path, error);
    if (!error) {
        node.size = size;
        node.hasSize = true;
    }

    filesystem::file_time_type time = filesystem::last_write_time(path, error);
    if (!error) {
        node.modified = toEpochSeconds(time);
        node.hasTime = true;
    }

    return node;
}
