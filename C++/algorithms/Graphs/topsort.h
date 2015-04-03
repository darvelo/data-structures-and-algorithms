#ifndef GRAPH_TOPSORT_H
#define GRAPH_TOPSORT_H

#include "../../data-structures/Graph.h"
#include "../../utils.h"

template <typename Data>
std::vector<typename Graph<Data>::Vertex*>
Graph<Data>::topSort() {
    if (!directed) {
        throw CustomException("Can't do topological sort on an undirected graph!");
    }

    std::vector<Vertex*> sorted;
    std::queue<Vertex*> q;
    std::queue<std::pair<Vertex*, int>> indegrees;

    std::for_each(begin(), end(), [&indegrees, &q] (Vertex& v) {
        // keep original record of all indegrees
        // since we'll need to modify them later
        indegrees.push({ &v, v.indegree() });

        // gather starting vertices
        if (v.indegree() == 0) {
            q.push(&v);
        }
    });

    while (!q.empty()) {
        Vertex* v = q.front();
        // since we're processing it now, pop the Vertex*
        // off the queue and into the final result
        q.pop();
        sorted.push_back(v);

        std::for_each(v->edges.begin(), v->edges.end(), [&q] (Vertex* w) {
            if (--w->_indegree == 0) {
                q.push(w);
            }
        });
    }

    if (sorted.size() != nvertices()) {
        throw CustomException("Cycle detected during topological sort!");
    }

    // restore original indegrees
    while (!indegrees.empty())  {
        auto& pr = indegrees.front();
        pr.first->_indegree = pr.second;
        indegrees.pop();
    }

    return sorted;
}

#endif
