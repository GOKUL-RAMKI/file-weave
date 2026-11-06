#include "GraphBuilder.h"

#include <cstddef>
#include <utility>

#include "InvertedIndex.h"
#include "RelationshipScorer.h"
#include "Tokenizer.h"

using namespace std;

namespace {

constexpr size_t maxPostings = 200;

}

Graph buildGraph(vector<FileNode> nodes, double threshold) {
    Graph graph;
    graph.nodes = move(nodes);

    InvertedIndex index;
    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        FileNode& node = graph.nodes[i];
        node.id = static_cast<int>(i);
        node.stem = Tokenizer::stem(node.name);
        node.tokens = Tokenizer::tokens(node.stem);

        for (const string& token : node.tokens) {
            index.add(token, node.id);
        }
        string prefix = Tokenizer::prefix(node.stem);
        if (!prefix.empty()) {
            index.add("~" + prefix, node.id);
        }
    }

    for (const auto& pair : index.pairs(maxPostings)) {
        RelationshipEdge edge = scorePair(graph.nodes[pair.first], graph.nodes[pair.second]);
        if (edge.score >= threshold) {
            graph.edges.push_back(edge);
        }
    }

    return graph;
}
