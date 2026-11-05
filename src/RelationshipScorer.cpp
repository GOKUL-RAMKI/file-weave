#include "RelationshipScorer.h"

#include "SimilarityAnalyzer.h"

RelationshipEdge scorePair(const FileNode& a, const FileNode& b) {
    RelationshipEdge edge;
    edge.a = a.id;
    edge.b = b.id;
    edge.jaccard = jaccard(a.tokens, b.tokens);
    edge.edit = editSimilarity(a.stem, b.stem);
    edge.path = pathProximity(a.dir, b.dir);
    edge.metadata = metadataSimilarity(a, b);
    edge.score = Scoring::weightJaccard * edge.jaccard
        + Scoring::weightEdit * edge.edit
        + Scoring::weightPath * edge.path
        + Scoring::weightMetadata * edge.metadata;
    edge.cost = 1.0 - edge.score;
    return edge;
}
