#include <iostream>
#include <stack>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::stack;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;

void processLate (stack<Graph<Data>::Vertex*>* sorted, Graph<Data>::Vertex& v) {
    sorted->push(&v);
}

void processEdge (Graph<Data>* g, Graph<Data>::Vertex& v, Graph<Data>::Vertex& w) {
    auto type = g->edgeClassification(v, w);

    if (type == g->BACK_EDGE) {
        cout << "Warning: directed cycle found, not a DAG." << endl;
        g->finished = true;
    }
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    stack<Graph<Data>::Vertex*> sorted;

    readIntoGraph(g, "topsort_graph.txt");

    auto pLate = bind(processLate, &sorted, _1);
    auto pEdge = bind(processEdge, &g, _1, _2);

    if (!g.directed) {
        throw CustomException("Can't do topological sort on an undirected graph!");
    }

    for (auto& v : g) {
        if (!v.discovered) {
            g.dfs(&v, nullptr, pLate, pEdge);
        }
    }

    cout << "Topological Sort: ";

    if (sorted.size()) {
        cout << sorted.top()->name;
        sorted.pop();
    } else {
        cout << "failed.";
    }

    while (!sorted.empty()) {
        cout << " -> " << sorted.top()->name;
        sorted.pop();
    }

    cout << endl;

    return 0;
}
