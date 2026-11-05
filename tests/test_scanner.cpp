#include <filesystem>
#include <fstream>
#include <set>
#include <string>

#include "FileScanner.h"
#include "check.h"

namespace fs = std::filesystem;

int main() {
    fs::path base = fs::temp_directory_path() / "frgv_test_scanner";
    std::error_code error;
    fs::remove_all(base, error);
    fs::create_directories(base / "sub" / "deep", error);
    fs::create_directories(base / "empty", error);

    std::ofstream(base / "Report_Final.TXT") << "a";
    std::ofstream(base / "sub" / "data.csv") << "b";
    std::ofstream(base / "sub" / "deep" / "image.png") << "c";
    std::ofstream(base / "sub" / "deep" / "store.db") << "d";

    ScanResult scan = scanDirectory(base.u8string());
    check(scan.files.size() == 4, "four files found");
    check(scan.skipped == 0, "no skipped entries");

    std::set<std::string> extensions;
    std::set<std::string> paths;
    for (const FileNode& file : scan.files) {
        extensions.insert(file.extension);
        paths.insert(file.path);
        check(fs::u8path(file.path).is_absolute(), "path is absolute");
        check(file.hasSize, "size available");
        check(file.hasTime, "modified time available");
    }
    check(paths.size() == scan.files.size(), "paths unique");
    check(extensions.count("txt") == 1, "extension lowercased");
    check(extensions.count("csv") == 1, "csv extension found");
    check(extensions.count("png") == 1, "png extension found");
    check(extensions.count("db") == 1, "db extension found");

    ScanResult empty = scanDirectory((base / "empty").u8string());
    check(empty.files.empty(), "empty directory has no files");
    check(empty.skipped == 0, "empty directory has no errors");

    ScanResult missing = scanDirectory((base / "missing").u8string());
    check(missing.files.empty(), "missing directory has no files");
    check(missing.skipped >= 1, "missing directory reported as skipped");

    fs::remove_all(base, error);
    return 0;
}
