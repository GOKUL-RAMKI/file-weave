#include "SimilarityAnalyzer.h"

#include <algorithm>
#include <cstddef>
#include <unordered_set>

#include "Tokenizer.h"

double jaccard(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    std::unordered_set<std::string> setA(a.begin(), a.end());
    std::unordered_set<std::string> setB;
    int shared = 0;
    for (const std::string& token : b) {
        if (setB.insert(token).second && setA.count(token)) ++shared;
    }

    int combined = static_cast<int>(setA.size() + setB.size()) - shared;
    if (combined == 0) return 0;
    return static_cast<double>(shared) / combined;
}

double editSimilarity(const std::string& a, const std::string& b) {
    if (a.empty() && b.empty()) return 1;

    std::size_t lenA = a.size();
    std::size_t lenB = b.size();
    std::vector<std::size_t> row(lenB + 1);
    for (std::size_t j = 0; j <= lenB; ++j) row[j] = j;

    for (std::size_t i = 1; i <= lenA; ++i) {
        std::size_t previous = row[0];
        row[0] = i;
        for (std::size_t j = 1; j <= lenB; ++j) {
            std::size_t old = row[j];
            std::size_t cost = a[i - 1] == b[j - 1] ? 0 : 1;
            row[j] = std::min({row[j] + 1, row[j - 1] + 1, previous + cost});
            previous = old;
        }
    }

    double distance = static_cast<double>(row[lenB]);
    double length = static_cast<double>(std::max(lenA, lenB));
    return 1.0 - distance / length;
}

namespace {

std::vector<std::string> splitPath(const std::string& dir) {
    std::vector<std::string> parts;
    std::string current;
    for (char c : dir) {
        if (c == '/' || c == '\\') {
            if (!current.empty()) {
                parts.push_back(Tokenizer::lower(current));
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) parts.push_back(Tokenizer::lower(current));
    return parts;
}

}

double pathProximity(const std::string& dirA, const std::string& dirB) {
    std::vector<std::string> a = splitPath(dirA);
    std::vector<std::string> b = splitPath(dirB);
    if (a.empty() && b.empty()) return 0;

    std::size_t shared = 0;
    std::size_t limit = std::min(a.size(), b.size());
    while (shared < limit && a[shared] == b[shared]) ++shared;

    return static_cast<double>(shared) / static_cast<double>(std::max(a.size(), b.size()));
}

double metadataSimilarity(const FileNode& a, const FileNode& b) {
    double extension = a.extension == b.extension ? 1.0 : 0.0;
    if (!a.hasSize || !b.hasSize) return extension;

    std::uintmax_t largest = std::max(a.size, b.size);
    double sizes = largest == 0 ? 1.0 : static_cast<double>(std::min(a.size, b.size)) / static_cast<double>(largest);
    return (extension + sizes) / 2.0;
}
