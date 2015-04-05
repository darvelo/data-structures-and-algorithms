#include <iostream>
#include <string>
#include <map>
#include <assert.h>
#include "../../data-structures/Graph.h"
#include "helpers.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct mapCompare {
    bool operator() (typename Graph<Data>::Vertex* v, typename Graph<Data>::Vertex* w) {
        return v->name < w->name;
    }
};

std::map<typename Graph<Data>::Vertex*, int, mapCompare> distances;

void processEarly(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed early" << endl;
}

void processLate(typename Graph<Data>::Vertex& v) {
    cout << v.name << " was processed late" << endl;

    if (!v.parent) {
        distances.insert({ &v, 0 });
    } else {
        distances.insert({ &v, distances[v.parent]+1 });
    }
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

    for (auto& pr : distances) {
        cout << pr.first->name << " distance from v1: " << pr.second << endl;
    }

    shortestPath(start, end, path);
    cout << "Shortest path from v1 to v6: " << path << endl;
    assert(path == "v1 -> v4 -> v6");

    return 0;
}
