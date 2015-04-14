#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::set;
using std::unordered_map;
using std::function;
using std::bind;
using std::ref;
using namespace std::placeholders;

template <typename T>
struct vertexNameCompare {
    bool operator()(T* v, T* w) const {
        return v->name < w->name;
    }
};

template <typename GraphT>
struct ArticulationResult {
    using Vertex = typename GraphT::Vertex;

    ArticulationResult(GraphT& _g) : g(_g) { };

    void processEarly(Vertex& v) {
        reachable_ancestor[&v] = &v;
    }

    void processLate(Vertex& v) {
        // if v is the root...
        if (!v.parent) {
            if (tree_outdegree[&v] > 1) {
                cout << "root articulation vertex: " << v.name << endl;
                root.insert(&v);
            }
            return;
        }

        // is v.parent the root of the DFS tree?
        bool isParentRoot = !v.parent->parent;

        if (!isParentRoot) {
            if (reachable_ancestor[&v] == v.parent) {
                cout << "parent articulation vertex: " << v.parent->name << " -> " << v.name << endl;
                parent.insert(v.parent);
            }

            if (reachable_ancestor[&v] == &v) {
                cout << "bridge articulation vertex (parent): " << v.parent->name << " -> " << v.name << endl;
                bridge.insert(v.parent);

                if (tree_outdegree[&v] > 0) {
                    cout << "bridge articulation vertex: " << v.name << endl;
                    bridge.insert(&v);
                }
            }
        }

        // earliest reachable time for v
        int time_v = reachable_ancestor[&v]->entryTime;
        // earliest reachable time for v.parent
        int time_parent = reachable_ancestor[v.parent]->entryTime;

        if (time_v < time_parent) {
            reachable_ancestor[v.parent] = reachable_ancestor[&v];
        }
    }

    void processEdge(Vertex& v, Vertex& w) {
        auto type = g.edgeClassification(v, w);

        if (type == g.TREE_EDGE) {
            tree_outdegree[&v] += 1;
        }

        if (type == g.BACK_EDGE && v.parent != &w) {
            if (w.entryTime < reachable_ancestor[&v]->entryTime) {
                reachable_ancestor[&v] = &w;
            }
        }
    }

    GraphT& g;
    unordered_map<Vertex*, Vertex*> reachable_ancestor;
    unordered_map<Vertex*, int> tree_outdegree;
    // types of articulation vertices
    set<Vertex*, vertexNameCompare<Vertex>> root;
    set<Vertex*, vertexNameCompare<Vertex>> bridge;
    set<Vertex*, vertexNameCompare<Vertex>> parent;
};

template <typename GraphT>
ArticulationResult<GraphT>
find_articulation_vertices(GraphT& g) {
    g.initializeSearch();

    ArticulationResult<GraphT> result(g);
    auto pEarly = bind(&ArticulationResult<GraphT>::processEarly, ref(result), _1);
    auto pLate = bind(&ArticulationResult<GraphT>::processLate, ref(result), _1);
    auto pEdge = bind(&ArticulationResult<GraphT>::processEdge, ref(result), _1, _2);

    g.dfs("v1", pEarly, pLate, pEdge);

    return result;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "input/articulation_vertices_graph.txt");

    auto result = find_articulation_vertices(g);
    g.print();

    cout << "Reachable Ancestors: " << endl;
    for (auto pr : result.reachable_ancestor) {
        cout << pr.first->name << " -> " << pr.second->name << endl;
    }
    cout << endl;

    cout << "Outdegrees: " << endl;
    for (auto pr : result.tree_outdegree) {
        cout << pr.first->name << " -> " << pr.second << endl;
    }
    cout << endl;

    cout << "Root Articulation Vertex:" << endl;
    for (auto root : result.root) {
        cout << root->name << endl;
    }
    cout << endl;

    cout << "Parent Articulation Vertices:" << endl;
    for (auto parent : result.parent) {
        cout << parent->name << endl;
    }
    cout << endl;

    cout << "Bridge Articulation Vertices:" << endl;
    for (auto bridgeV : result.bridge) {
        cout << bridgeV->name << endl;
    }
    cout << endl;

    return 0;
}
