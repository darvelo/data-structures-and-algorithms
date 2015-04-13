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
struct TwoColorResult {
    using Vertex = typename GraphT::Vertex;

    enum Color {
        BLACK,
        WHITE,
        UNCOLORED
    };

    TwoColorResult(GraphT& g) {
        for (auto& v : g) {
            colors[&v] = UNCOLORED;
        }
    }

    void processEdge(Vertex& v, Vertex& w) {
        if (colors[&v] == colors[&w]) {
            cout << "Warning: Graph is not bipartite due to"
                 << "(" << v.name << "," << w.name << ")" << endl;
        }

        colors[&w] = complement(colors[&v]);
    }

    static Color complement(Color c) {
        if (c == BLACK) return WHITE;
        if (c == WHITE) return BLACK;
        return UNCOLORED;
    }

    unordered_map<Vertex*, Color> colors;
};

template <typename GraphT>
TwoColorResult<GraphT>
twocolor(GraphT& g) {
    g.initializeSearch();

    TwoColorResult<GraphT> result(g);
    auto pEdge = bind(&TwoColorResult<GraphT>::processEdge, ref(result), _1, _2);

    for (auto& v : g) {
        if (!v.discovered) {
            result.colors[&v] = result.WHITE;
            g.bfs(&v, nullptr, nullptr, pEdge);
        }
    }

    return result;
}

int main() {
    bool directed = false;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/two_color_graph.txt");
    g.print();

    auto result = twocolor(g);

    for (auto pr : result.colors) {
        cout << pr.first->name << ": " << pr.second << endl;
    }

    return 0;
}
