#include <iostream>
#include <unordered_map>
#include <functional>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::unordered_map;
using std::bind;
using std::ref;
using namespace std::placeholders;

template <typename GraphT>
struct ConnectedResult {
    using Vertex = typename GraphT::Vertex;

    void processEarly(Vertex& v) {
        components[&v] = componentsFound;
    }

    int componentsFound = 0;
    unordered_map<Vertex*, int> components;
};

template <typename GraphT>
ConnectedResult<GraphT>
connected_components(GraphT& g) {
    g.initializeSearch();

    ConnectedResult<GraphT> result;
    auto pEarly = bind(&ConnectedResult<GraphT>::processEarly, ref(result), _1);

    for (auto& v : g) {
        if (!v.discovered) {
            ++result.componentsFound;
            g.bfs(&v, pEarly);
        }
    }

    return result;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/connected_components_graph.txt");

    auto result = connected_components(g);

    for (auto pr : result.components) {
        cout << pr.first->name << ": " << pr.second << endl;
    }

    return 0;
}
