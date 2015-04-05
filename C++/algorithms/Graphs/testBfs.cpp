#include <iostream>
#include <string>
#include <assert.h>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

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
    if (start != end && end->parent) {
        shortestPath(start, end->parent, path);
        path += " -> ";
    }
    path += end->name;
}

int main() {
    Graph<Data> g(true);
    readIntoGraph(g, "bfs_graph.txt");
    g.print();
    g.bfs("v1", processEarly, processLate, processEdge);
    cout << endl;
    g.print();

    string path;
    auto start = &*g.getVertex("v1");
    auto end = &*g.getVertex("v6");

    shortestPath(start, end, path);
    cout << "Shortest path from v1 to v6: " << path << endl;
    assert(path == "v1 -> v4 -> v6");

    return 0;
}
