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
using namespace std::placeholders;

typedef Graph<Data>::Vertex Vertex;

struct TopSortResult {
    TopSortResult(Graph<Data>& _g) : g(_g) { };

    void processVertexLate (Vertex& v) {
        result.push(&v);
    }

    void processEdge (Vertex& v, Vertex& w) {
        auto type = g.edgeClassification(v, w);

        if (type == g.BACK_EDGE) {
            cout << "Warning: directed cycle found, not a DAG." << endl;
            g.finished = true;
        }
    }

    stack<Vertex*> result;
    Graph<Data>& g;
};

int main() {
    bool directed = true;
    Graph<Data> g(directed);

    if (!g.directed) {
        throw CustomException("Can't do topological sort on an undirected graph!");
    }

    readIntoGraph(g, "input/topsort_graph.txt");

    TopSortResult t(g);
    auto& result = t.result;
    auto pLate = bind(&TopSortResult::processVertexLate, ref(t), _1);
    auto pEdge = bind(&TopSortResult::processEdge, ref(t), _1, _2);

    for (auto& v : g) {
        if (!v.discovered) {
            g.dfs(&v, nullptr, pLate, pEdge);
        }
    }

    cout << "Topological Sort: ";

    if (g.finished || result.empty()) {
        cout << "failed." << endl;
        return 1;
    }

    cout << result.top()->name;
    result.pop();

    while (!result.empty()) {
        cout << " -> " << result.top()->name;
        result.pop();
    }

    cout << endl;

    return 0;
}
