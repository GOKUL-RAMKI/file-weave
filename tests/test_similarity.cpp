#include "SimilarityAnalyzer.h"
#include "check.h"

int main() {
    checkNear(jaccard({"a", "b"}, {"a", "b"}), 1.0, "identical token sets");
    checkNear(jaccard({"a", "b"}, {"c", "d"}), 0.0, "disjoint token sets");
    checkNear(jaccard({}, {}), 0.0, "empty token sets");
    checkNear(jaccard({"a", "a", "b"}, {"b", "b"}), 0.5, "duplicate tokens counted once");

    checkNear(editSimilarity("", ""), 1.0, "empty strings are similar");
    checkNear(editSimilarity("abc", "abc"), 1.0, "identical strings");
    checkNear(editSimilarity("report", "reprot"), 1.0 - 2.0 / 6.0, "transposed letters");
    checkNear(editSimilarity("abc", ""), 0.0, "empty against non-empty");

    checkNear(pathProximity("D:/work/a", "D:/work/a"), 1.0, "same directory");
    checkNear(pathProximity("D:/work/a", "D:/work/b"), 2.0 / 3.0, "sibling directories");
    checkNear(pathProximity("D:/work/a", "C:/work/a"), 0.0, "different drives");

    FileNode first;
    first.extension = "txt";
    first.hasSize = true;
    first.size = 100;

    FileNode same;
    same.extension = "txt";
    same.hasSize = true;
    same.size = 100;

    FileNode half;
    half.extension = "txt";
    half.hasSize = true;
    half.size = 50;

    FileNode other;
    other.extension = "csv";
    other.hasSize = true;
    other.size = 50;

    FileNode unknown;
    unknown.extension = "txt";

    checkNear(metadataSimilarity(first, same), 1.0, "identical metadata");
    checkNear(metadataSimilarity(first, half), 0.75, "same type, half size");
    checkNear(metadataSimilarity(first, other), 0.25, "different type, half size");
    checkNear(metadataSimilarity(first, unknown), 1.0, "missing size falls back to extension");

    return 0;
}
