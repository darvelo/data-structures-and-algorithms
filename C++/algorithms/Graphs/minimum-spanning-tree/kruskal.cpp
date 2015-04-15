#include <iostream>
#include <vector>
#include <algorithm>
#include "../../../data-structures/Graph.h"
#include "../../../data-structures/DisjointSets.h"
#include "../helpers.h"

using std::cout;
using std::endl;
using std::vector;
using std::sort;

template <typename GraphT>
struct EdgePair {
    using Vertex = typename GraphT::Vertex;

    EdgePair(Vertex* _v, Vertex* _w, int _weight)
        : v{_v}, w{_w}, weight{_weight} { }
    Vertex* v;
    Vertex* w;
    int weight;
};

template <typename GraphT>
struct EdgeCompare {
    bool operator()(EdgePair<GraphT>& a, EdgePair<GraphT>& b) {
        return a.weight < b.weight;
    }
};

template <typename GraphT>
vector<EdgePair<GraphT>>
to_edge_vector(GraphT& g) {
    vector<EdgePair<GraphT>> edges;

    for (auto& v : g) {
        for (auto& e : v.edges) {
            edges.push_back({ &v, e.w, e.weight });
        }
    }

    return edges;
}

template <typename GraphT>
vector<EdgePair<GraphT>>
kruskal(GraphT& g) {
    using Vertex = typename GraphT::Vertex;

    DisjointSets<Vertex*> s;
    vector<EdgePair<GraphT>> result;
    auto edges = to_edge_vector(g);

    sort(edges.begin(), edges.end(), EdgeCompare<GraphT>());

    for (int i = 0; i < g.nedges(); ++i) {
        // ignore vertices that are parts of the same component (and thus create a cycle)
        // we only want to connect trees together and so only consider tree edges
        if (!s.sameRoot(edges[i].v, edges[i].w)) {
            // connect trees represented by v and w
            s.unionSets(edges[i].v, edges[i].w);
            result.push_back(edges[i]);
        }
    }

    return result;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/minimum_spanning_tree_graph.txt");
    g.print();

    auto edges = kruskal(g);

    for (auto& e : edges) {
        cout << "Edge ("
             << e.v->name << "," << e.w->name
             << ") "
             << "weight: " << e.weight
             << endl;
    }

    return 0;
}
