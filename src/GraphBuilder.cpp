#include "GraphBuilder.h"

#include <cstddef>
#include <utility>

#include "InvertedIndex.h"
#include "RelationshipScorer.h"
#include "Tokenizer.h"

namespace {

constexpr std::size_t maxPostings = 200;

}

Graph buildGraph(std::vector<FileNode> nodes, double threshold) {
    Graph graph;
    graph.nodes = std::move(nodes);

    InvertedIndex index;
    for (std::size_t i = 0; i < graph.nodes.size(); ++i) {
        FileNode& node = graph.nodes[i];
        node.id = static_cast<int>(i);
        node.stem = Tokenizer::stem(node.name);
        node.tokens = Tokenizer::tokens(node.stem);

        for (const std::string& token : node.tokens) {
            index.add(token, node.id);
        }
        std::string prefix = Tokenizer::prefix(node.stem);
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
