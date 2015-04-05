#ifndef GRAPH_BFS_H
#define GRAPH_BFS_H

#include <queue>
#include <string>
#include <algorithm>
#include "../../data-structures/Graph.h"
#include "../../utils.h"

template <typename Data>
void
Graph<Data>::bfs(std::string start,
                 void(*processEarly)(Vertex& v),
                 void(*processLate)(Vertex& v),
                 void(*processEdge)(Vertex& v, Vertex& w))
{
    auto itr = getVertex(start);

    if (itr == end()) {
        throw CustomException("Can't start BFS on non-existent vertex!");
    }

    initializeSearch();

    Vertex* v = &*itr;
    v->discovered = true;

    std::queue<Vertex*> q;
    q.push(v);

    while (!q.empty()) {
        v = q.front();
        q.pop();

        if (processEarly) {
            processEarly(*v);
        }

        v->processed = true;
        bool directed = directed;

        std::for_each(v->edges.begin(), v->edges.end(), [&q, directed, v, processEdge] (Vertex* w) {
            if (processEdge && (!w->processed || directed)) {
                processEdge(*v, *w);
            }

            if (!w->discovered) {
                q.push(w);
                w->discovered = true;
                w->parent = v;
            }
        });

        if (processLate) {
            processLate(*v);
        }
    }
}

#endif
