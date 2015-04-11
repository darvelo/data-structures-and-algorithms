#include <iostream>
#include <stack>
#include <unordered_map>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::stack;
using std::unordered_map;
using std::function;
using std::ref;
using std::bind;
using namespace std::placeholders;

typedef Graph<Data>::Vertex Vertex;

struct ComponentsResult {
    ComponentsResult(Graph<Data>& _g) : g(_g) {
        for (auto& v : g) {
            low[&v] = &v;
            component[&v] = -1;
        }
    };

    void pop_component(Vertex& v) {
        componentsFound++;

        while (active.top() != &v) {
            component[active.top()] = componentsFound;
            active.pop();
        }

        // completely assign the component by also processing v
        component[active.top()] = componentsFound;
        active.pop();
    }

    void processEarly(Vertex& v) {
        active.push(&v);
    }

    void processLate(Vertex& v) {
        if (low[&v] == &v) {
            // this signifies the cutoff of the newest component
            pop_component(v);
        }

        if (v.parent && low[&v]->entryTime < low[v.parent]->entryTime) {
            low[v.parent] = low[&v];
        }
    }

    void processEdge(Vertex& v, Vertex& w) {
        auto type = g.edgeClassification(v, w);

        if (type == g.BACK_EDGE) {
            if (w.entryTime < low[&v]->entryTime) {
                low[&v] = &w;
            }
        }

        if (type == g.CROSS_EDGE) {
            if (component[&w] == -1 && w.entryTime < low[&v]->entryTime) {
                low[&v] = &w;
            }
        }
    }

    int componentsFound = 0;
    Graph<Data>& g;
    stack<Vertex*> active;
    // oldest vertex surely in component of v
    unordered_map<Vertex*, Vertex*> low;
    // strong component number for each vertex
    unordered_map<Vertex*, int> component;
};

unordered_map<Vertex*, int>
strongly_connected_components(Graph<Data>& g) {
    if (!g.directed) {
        throw CustomException("Finding strongly-connected components requires a directed graph!");
    }

    g.initializeSearch();

    ComponentsResult result(g);

    auto pEarly = bind(&ComponentsResult::processEarly, ref(result), _1);
    auto pLate = bind(&ComponentsResult::processLate, ref(result), _1);
    auto pEdge = bind(&ComponentsResult::processEdge, ref(result), _1, _2);

    for (auto& v : g) {
        if (!v.discovered) {
            g.dfs(&v, pEarly, pLate, pEdge);
        }
    }

    return result.component;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/strongly_connected_components_graph.txt");

    auto result = strongly_connected_components(g);
    g.print();

    for (std::pair<Vertex*, int> pr : result) {
        cout << pr.first->name << " -> " << pr.second << endl;
    }

    return 0;
}
