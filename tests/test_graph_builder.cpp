#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "GraphBuilder.h"
#include "check.h"

using namespace std;

namespace {

FileNode makeNode(const string& name, const string& dir, uintmax_t size) {
    FileNode file;
    file.name = name;
    file.path = dir + "/" + name;
    file.dir = dir;
    file.size = size;
    file.hasSize = true;
    size_t dot = name.find_last_of('.');
    file.extension = dot == string::npos ? string() : name.substr(dot + 1);
    return file;
}

int countEdges(const Graph& graph, int a, int b) {
    int count = 0;
    for (const RelationshipEdge& edge : graph.edges) {
        if ((edge.a == a && edge.b == b) || (edge.a == b && edge.b == a)) ++count;
    }
    return count;
}

}

int main() {
    Graph empty = buildGraph({}, 0.5);
    check(empty.nodes.empty() && empty.edges.empty(), "empty graph");

    Graph single = buildGraph({ makeNode("report.txt", "D:/work", 10) }, 0.5);
    check(single.nodes.size() == 1, "single node kept");
    check(single.nodes[0].id == 0, "node id assigned");
    check(!single.nodes[0].tokens.empty(), "node tokenized");
    check(single.edges.empty(), "single node has no edges");

    Graph similar = buildGraph({
        makeNode("report_final.txt", "D:/work", 10),
        makeNode("report_final_v2.txt", "D:/work", 10)
    }, 0.5);
    check(similar.edges.size() == 1, "similar names produce one edge");
    checkNear(similar.edges[0].score, 1.0 - similar.edges[0].cost, "cost inverts score");

    Graph near = buildGraph({
        makeNode("alpha.txt", "D:/work", 10),
        makeNode("alpine.txt", "D:/work", 10)
    }, 0.4);
    check(near.edges.size() == 1, "prefix bucket creates candidate pair");

    Graph filtered = buildGraph({
        makeNode("alpha.txt", "D:/work", 10),
        makeNode("alpine.txt", "D:/work", 10)
    }, 0.5);
    check(filtered.edges.empty(), "threshold rejects weak pair");

    Graph unrelated = buildGraph({
        makeNode("alpha.txt", "D:/work", 10),
        makeNode("zulu.csv", "D:/other", 20)
    }, 0.0);
    check(unrelated.edges.empty(), "no shared token means no candidate pair");

    Graph shared = buildGraph({
        makeNode("shared_a.txt", "D:/work", 10),
        makeNode("shared_b.txt", "D:/work", 10),
        makeNode("shared_c.txt", "D:/work", 10)
    }, 0.5);
    check(shared.edges.size() == 3, "all candidate pairs scored");
    set<pair<int, int>> unique;
    for (const RelationshipEdge& edge : shared.edges) {
        check(edge.a != edge.b, "no self edge");
        unique.insert({min(edge.a, edge.b), max(edge.a, edge.b)});
    }
    check(unique.size() == shared.edges.size(), "no duplicate edges");
    check(countEdges(shared, 0, 1) == 1, "pair stored once");

    return 0;
}
