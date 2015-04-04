#include <iostream>
#include <string>
#include <stack>
#include <assert.h>
#include "../../data-structures/Graph.h"
#include "./testDefs.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::stack;

void processEarly(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed early" << endl;
}

void processLate(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed late" << endl;
}

void processEdge(typename Graph<Data>::Vertex& v, typename Graph<Data>::Vertex& w) {
    cout << v.name << " edge to " << w.name << endl;
}

void shortestPath(typename Graph<Data>::Vertex* start, typename Graph<Data>::Vertex* end, string& path) {
    if (!start) {
        return;
    }

    shortestPath(start->parent, end, path);
    path += start->name + " ";
}

int main() {
    Graph<Data> g(true);
    readIntoGraph(g, "bfs_graph.txt");
    g.print();
    g.bfs("v1", processEarly, processLate, processEdge);
    cout << endl;
    g.print();

    string path;
    auto start = &*g.getVertex("v6");
    auto end = &*g.getVertex("v1");

    shortestPath(start, end, path);
    assert(path == "v1 v4 v6 ");
    cout << path << endl;

    return 0;
}
