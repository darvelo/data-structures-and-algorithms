#include <iostream>
#include <string>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::string;

Graph<Data> g(true);

void processEarly(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed early" << endl;
}

void processLate(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed late" << endl;
}

void processEdge(typename Graph<Data>::Vertex& v, typename Graph<Data>::Vertex& w) {
    cout << v.name << " edge to " << w.name << endl;
}

void findPath(typename Graph<Data>::Vertex* start, typename Graph<Data>::Vertex* end, string& path) {
    if (start != end && end->parent) {
        findPath(start, end->parent, path);
        path += " -> ";
    }
    path += end->name;
}

int main() {
    readIntoGraph(g, "bfs_graph.txt");
    g.initializeSearch();
    g.print();
    g.dfs("v1", processEarly, processLate, processEdge);
    cout << endl;
    g.print();

    string path;
    auto start = &*g.getVertex("v1");
    auto end = &*g.getVertex("v6");

    findPath(start, end, path);
    cout << "Path from v1 to v6: " << path << endl;

    return 0;
}
