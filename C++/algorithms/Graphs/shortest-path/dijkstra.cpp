#include <iostream>
#include <string>
#include <unordered_map>
#include "../../../data-structures/Graph.h"
#include "../../../utils.h"
#include "../helpers.h"

using std::string;
using std::unordered_map;

static int MAX_INT = std::numeric_limits<int>::max();

template <typename GraphT>
void
dijkstra(GraphT& g, typename GraphT::Vertex& start) {
    using Vertex = typename GraphT::Vertex;

    g.initializeSearch();

    unordered_map<Vertex*, bool> intree;
    int weight;
    int dist;

    Vertex* v = &start;
    v->distance = 0;

    while (!intree[v]) {
        intree[v] = true;

        for (auto& e : v->edges) {
            auto w = e.w;
            weight = e.weight;

            if (w->distance > weight + v->distance) {
                w->distance = weight + v->distance;
                w->parent = v;
            }
        }

        dist = MAX_INT;

        for (auto& x : g) {
            if (!intree[&x] && (dist > x.distance)) {
                dist = x.distance;
                v = &x;
            }
        }
    }
}

template <typename GraphT>
void
dijkstra(GraphT& g, string start) {
    auto iter = g.getVertex(start);

    if (iter == g.end()) {
        throw CustomException("Can't find minimum spanning tree from non-existent vertex!");
    }

    dijkstra(g, *iter);
}

int main() {
    bool directed = false;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/spanning_tree_graph.txt");
    dijkstra(g, "A");
    g.print();
    return 0;
}
