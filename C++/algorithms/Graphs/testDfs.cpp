#include <iostream>
#include <string>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::endl;
using std::string;

typedef Graph<Data>::Vertex Vertex;

void processEarly(Vertex& v) {
    cout << v.name << " was processed early" << endl;
}

void processLate(Vertex& v) {
    cout << v.name << " was processed late" << endl;
}

void processEdge(Vertex& v, Vertex& w) {
    cout << v.name << " edge to " << w.name << endl;
}

void findPath(Vertex* start, Vertex* end, string& path) {
    if (start != end && end->parent) {
        findPath(start, end->parent, path);
        path += " -> ";
    }
    path += end->name;
}

int main() {
    bool directed = true;
    Graph<Data> g(directed);

    readIntoGraph(g, "input/dfs_graph.txt");

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
