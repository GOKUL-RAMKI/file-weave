#include "FileScanner.h"

#include <filesystem>
#include <system_error>

#include "MetadataCollector.h"

ScanResult scanDirectory(const std::string& root) {
    namespace fs = std::filesystem;

    ScanResult result;
    std::error_code error;
    fs::path start = fs::u8path(root);
    fs::recursive_directory_iterator it(start, fs::directory_options::skip_permission_denied, error);
    fs::recursive_directory_iterator end;

    if (error) {
        result.skipped = 1;
        return result;
    }

    while (it != end) {
        if (it->is_regular_file(error)) {
            result.files.push_back(collectMetadata(it->path()));
        } else if (error) {
            ++result.skipped;
            error.clear();
        }

        it.increment(error);
        if (error) {
            ++result.skipped;
            error.clear();
        }
    }

    return result;
}
