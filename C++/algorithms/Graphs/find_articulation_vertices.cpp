#include <iostream>
#include <unordered_map>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::unordered_map;
using std::function;
using std::bind;
using std::ref;
using namespace std::placeholders;

typedef Graph<Data>::Vertex Vertex;

struct ArticulationResult {
    ArticulationResult(Graph<Data>& _g) : g(_g) { };

    void processEarly(Vertex& v) {
        reachable_ancestor[&v] = &v;
    }

    void processLate(Vertex& v) {
        // if v is the root...
        if (!v.parent) {
            if (tree_outdegree[&v] > 1) {
                cout << "root articulation vertex: " << v.name << endl;
            }
            return;
        }

        // is v.parent the root of the DFS tree?
        bool isParentRoot = !v.parent->parent;

        if (!isParentRoot) {
            if (reachable_ancestor[&v] == v.parent) {
                cout << "parent articulation vertex: " << v.parent->name << " -> " << v.name << endl;
            }

            if (reachable_ancestor[&v] == &v) {
                cout << "bridge articulation vertex (parent): " << v.parent->name << " -> " << v.name << endl;

                if (tree_outdegree[&v] > 0) {
                    cout << "bridge articulation vertex: " << v.name << endl;
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

    Graph<Data>& g;
    unordered_map<Vertex*, Vertex*> reachable_ancestor;
    unordered_map<Vertex*, int> tree_outdegree;
};

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "input/articulation_verticies_graph.txt");

    ArticulationResult a(g);
    auto pEarly = bind(&ArticulationResult::processEarly, ref(a), _1);
    auto pLate = bind(&ArticulationResult::processLate, ref(a), _1);
    auto pEdge = bind(&ArticulationResult::processEdge, ref(a), _1, _2);

    g.dfs("v1", pEarly, pLate, pEdge);
    cout << endl;

    cout << "Reachable Ancestors: " << endl;
    for (std::pair<Vertex*, Vertex*> pr : a.reachable_ancestor) {
        cout << pr.first->name << " -> " << pr.second->name << endl;
    }
    cout << endl;

    cout << "Outdegrees: " << endl;
    for (std::pair<Vertex*, int> pr : a.tree_outdegree) {
        cout << pr.first->name << " -> " << pr.second << endl;
    }
    cout << endl;

    g.print();
    return 0;
}
