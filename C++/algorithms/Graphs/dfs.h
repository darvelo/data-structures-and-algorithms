#ifndef GRAPH_DFS_H
#define GRAPH_DFS_H

#include <string>
#include <functional>
#include "../../data-structures/Graph.h"
#include "../../utils.h"

template <typename Data>
void
Graph<Data>::dfs(Vertex* v,
                 std::function<void (Graph<Data>::Vertex&)> processEarly,
                 std::function<void (Graph<Data>::Vertex&)> processLate,
                 std::function<void (Graph<Data>::Vertex&, Graph<Data>::Vertex&)> processEdge)
{
    if (finished) return;

    v->discovered = true;
    v->entryTime = time++;

    if (processEarly) processEarly(*v);

    for (auto w : v->edges) {
        if (!w->discovered) {
            w->parent = v;
            w->distance = v->distance + 1;
            if (processEdge) processEdge(*v, *w);
            dfs(w, processEarly, processLate, processEdge);
        } else if ((!w->processed && v->parent != w) || directed) {
            if (processEdge) processEdge(*v, *w);
        }

        if (finished) return;
    }

    v->exitTime = time++;
    v->processed = true;
    if (processLate) processLate(*v);
}

template <typename Data>
void
Graph<Data>::dfs(Vertex& v,
                 std::function<void (Graph<Data>::Vertex&)> processEarly,
                 std::function<void (Graph<Data>::Vertex&)> processLate,
                 std::function<void (Graph<Data>::Vertex&, Graph<Data>::Vertex&)> processEdge)
{
    dfs(&v, processEarly, processLate, processEdge);
}

template <typename Data>
void
Graph<Data>::dfs(std::string start,
                 std::function<void (Graph<Data>::Vertex&)> processEarly,
                 std::function<void (Graph<Data>::Vertex&)> processLate,
                 std::function<void (Graph<Data>::Vertex&, Graph<Data>::Vertex&)> processEdge)
{
    auto itr = getVertex(start);

    if (itr == end()) {
        throw CustomException("Can't start DFS on non-existent vertex!");
    }

    Vertex* v = &*itr;
    v->distance = 0;

    dfs(v, processEarly, processLate, processEdge);
}

#endif
