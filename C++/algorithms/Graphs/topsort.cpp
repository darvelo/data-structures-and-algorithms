#include <iostream>
#include <stack>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::stack;
using std::bind;
using std::ref;
using std::placeholders::_1;
using std::placeholders::_2;

typedef Graph<Data>::Vertex Vertex;

void processLate (stack<Vertex*>& sorted, Vertex& v) {
    sorted.push(&v);
}

void processEdge (Graph<Data>& g, Vertex& v, Vertex& w) {
    auto type = g.edgeClassification(v, w);

    if (type == g.BACK_EDGE) {
        cout << "Warning: directed cycle found, not a DAG." << endl;
        g.finished = true;
    }
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    stack<Vertex*> sorted;

    readIntoGraph(g, "input/topsort_graph.txt");

    auto pLate = bind(processLate, ref(sorted), _1);
    auto pEdge = bind(processEdge, ref(g), _1, _2);

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
