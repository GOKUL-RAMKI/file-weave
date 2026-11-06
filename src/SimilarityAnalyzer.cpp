#include "SimilarityAnalyzer.h"

#include <algorithm>
#include <cstddef>
#include <unordered_set>

#include "Tokenizer.h"

using namespace std;

double jaccard(const vector<string>& a, const vector<string>& b) {
    unordered_set<string> setA(a.begin(), a.end());
    unordered_set<string> setB;
    int shared = 0;
    for (const string& token : b) {
        if (setB.insert(token).second && setA.count(token)) ++shared;
    }

    int combined = static_cast<int>(setA.size() + setB.size()) - shared;
    if (combined == 0) return 0;
    return static_cast<double>(shared) / combined;
}

double editSimilarity(const string& a, const string& b) {
    if (a.empty() && b.empty()) return 1;

    size_t lenA = a.size();
    size_t lenB = b.size();
    vector<size_t> row(lenB + 1);
    for (size_t j = 0; j <= lenB; ++j) row[j] = j;

    for (size_t i = 1; i <= lenA; ++i) {
        size_t previous = row[0];
        row[0] = i;
        for (size_t j = 1; j <= lenB; ++j) {
            size_t old = row[j];
            size_t cost = a[i - 1] == b[j - 1] ? 0 : 1;
            row[j] = min({row[j] + 1, row[j - 1] + 1, previous + cost});
            previous = old;
        }
    }

    double distance = static_cast<double>(row[lenB]);
    double length = static_cast<double>(max(lenA, lenB));
    return 1.0 - distance / length;
}

namespace {

vector<string> splitPath(const string& dir) {
    vector<string> parts;
    string current;
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

double pathProximity(const string& dirA, const string& dirB) {
    vector<string> a = splitPath(dirA);
    vector<string> b = splitPath(dirB);
    if (a.empty() && b.empty()) return 0;

    size_t shared = 0;
    size_t limit = min(a.size(), b.size());
    while (shared < limit && a[shared] == b[shared]) ++shared;

    return static_cast<double>(shared) / static_cast<double>(max(a.size(), b.size()));
}

double metadataSimilarity(const FileNode& a, const FileNode& b) {
    double extension = a.extension == b.extension ? 1.0 : 0.0;
    if (!a.hasSize || !b.hasSize) return extension;

    uintmax_t largest = max(a.size, b.size);
    double sizes = largest == 0 ? 1.0 : static_cast<double>(min(a.size, b.size)) / static_cast<double>(largest);
    return (extension + sizes) / 2.0;
}
