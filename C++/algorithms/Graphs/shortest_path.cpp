#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

template <typename Vertex>
void traverse(Vertex* start, Vertex* finish, vector<Vertex*>& s) {
    if (!finish || start->parent == finish) {
        return;
    }

    traverse(start, finish->parent, s);
    s.push_back(finish);
}

template <typename GraphT>
vector<typename GraphT::Vertex*>
shortestPath(GraphT& g, string start, string finish) {
    vector<typename GraphT::Vertex*> result;

    g.initializeSearch();
    g.bfs(start);

    auto iterStart = g.getVertex(start);

    if (iterStart == g.end()) {
        throw BadVertexException(finish);
    }

    auto iterFinish = g.getVertex(finish);

    if (iterFinish == g.end()) {
        throw BadVertexException(finish);
    }

    traverse(&*iterStart, &*iterFinish, result);
    return result;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "input/bfs_graph.txt");

    auto result = shortestPath(g, "v1", "v6");
    string path;

    for (auto v : result) {
        path += v->name;

        if (v != result.back()) {
            path += " -> ";
        }
    }

    cout << "Shortest path from v1 to v6: " << path << endl;
    assert(path == "v1 -> v4 -> v6");

    return 0;
}
