#ifndef GRAPH_BFS_H
#define GRAPH_BFS_H

#include <queue>
#include <string>
#include <algorithm>
#include <functional>
#include "../../data-structures/Graph.h"
#include "../../utils.h"

template <typename Data>
void
Graph<Data>::bfs(std::string start,
                 std::function<void (Vertex&)> processEarly,
                 std::function<void (Vertex&)> processLate,
                 std::function<void (Vertex&, Vertex&)> processEdge)
{
    auto itr = getVertex(start);

    if (itr == end()) {
        throw CustomException("Can't start BFS on non-existent vertex!");
    }

    initializeSearch();

    Vertex* v = &*itr;
    v->discovered = true;
    v->distance = 0;

    std::queue<Vertex*> q;
    q.push(v);

    while (!q.empty()) {
        if (finished) return;

        v = q.front();
        q.pop();

        if (processEarly) processEarly(*v);

        for (auto w : v->edges) {
            if (!w->discovered) {
                q.push(w);
                w->parent = v;
                w->distance = v->distance + 1;
                if (processEdge) processEdge(*v, *w);
                w->discovered = true;
            } else if ((!w->processed && v->parent != w) || directed) {
                if (processEdge) processEdge(*v, *w);
            }

            if (finished) return;
        }

        v->processed = true;
        if (processLate) processLate(*v);
    }
}

#endif
