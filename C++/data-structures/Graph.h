#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <functional>
#include <forward_list>
#include <unordered_map>
#include <algorithm>
#include <limits>

static int max_int = std::numeric_limits<int>::max();

template <typename Data>
class Graph {
public:
    /* constants */
    enum edge_t {
        TREE_EDGE,
        BACK_EDGE,
        FORWARD_EDGE,
        CROSS_EDGE,
        UNCLASSIFIED_EDGE
    };

    class Vertex {
    public:
        void print(std::ostream& out = std::cout) {
            out << "Vertex: " << name
                << ", Weight: " << weight
                << ", Indegree: " << _indegree
                << ", Parent: " << (parent ? parent->name : "NONE")
                << ", Distance: " << distance
                << ", Entry Time: " << entryTime
                << ", Exit Time: " << exitTime
                << ", Edges:"
                << std::endl;
            for (auto& v : edges) {
                out << "    \"" << v->name << "\" " << v->weight << std::endl;
            }
        }

        // inherent properties
        const std::string name;
        int weight = 1;
        int indegree() { return _indegree; }

        // search parameters, are reset as needed
        Vertex* parent = nullptr;
        bool discovered = false;
        bool processed = false;
        int distance = max_int;
        int entryTime = max_int;
        int exitTime = max_int;

        // user-supplied data
        Data* data = nullptr;
    private:
        explicit Vertex(std::string _name, int _weight = 1, Data* _data = nullptr)
            : name(_name), weight(_weight), data(_data) { }

        std::forward_list<Vertex*> edges;
        int _indegree = 0;
        friend Graph<Data>;
    };

    // wrap unordered_map iterators to produce a Vertex&
    // rather than the unordered_map default, a pair
    class iterator {
    public:
        Vertex& operator*() {
            return *current->second;
        }

        iterator& operator++() {
            ++current;
            return *this;
        }

        iterator& operator++(int) {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator& operator--() {
            --current;
            return *this;
        }

        iterator operator--(int) {
            iterator old = *this;
            --(*this);
            return old;
        }

        bool operator==(const iterator& rhs) {
            return current == rhs.current;
        }

        bool operator!=(const iterator& rhs) {
            return !(current == rhs.current);
        }
    private:
        iterator(typename std::unordered_map<std::string, Vertex*>::iterator iter)
            : current(iter) { }

        typename std::unordered_map<std::string, Vertex*>::iterator current;
        friend Graph<Data>;
    };
public:
    /* algorithms - defined elsewhere */
    std::vector<Vertex*> topSort();
    void bfs(std::string start,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&, Vertex&)> = nullptr);
    void dfs(std::string start,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&, Vertex&)> = nullptr);

    /* implementation */
    Graph(bool _directed = false) : directed(_directed) { }

    void initializeSearch() {
        time = 0;
        finished = false;

        std::for_each(begin(), end(), [] (Vertex& v) {
            v.parent = nullptr;
            v.discovered = false;
            v.processed = false;
            v.distance = v.entryTime = v.exitTime = max_int;
        });
    }

    Vertex& addVertex(std::string name, int weight = 1, Data* data = nullptr) {
        Vertex* v = new Vertex{name, weight, data};
        auto result = vertices.insert({ name, v });

        // if insert failed, map already had such a key.
        // grab the element associated with that key
        if (!result.second) {
            delete v;
            v = result.first->second;
        }

        return *v;
    }

    iterator getVertex(std::string name) {
        auto iter = vertices.find(name);

        if (iter == vertices.end()) {
            return end();
        }

        return iterator(iter);
    }

    bool addEdge(std::string from, std::string to) {
        iterator source = getVertex(from);
        iterator theEnd = end();

        if (source == theEnd) {
            return false;
        }

        iterator sink = getVertex(to);

        if (sink == theEnd) {
            return false;
        }

        auto& srcEdges = (*source).edges;

        bool exists = std::any_of(srcEdges.begin(), srcEdges.end(), [&to] (Vertex* v) {
            return v->name == to;
        });

        if (!exists) {
            ++nEdges;
            ++(*sink)._indegree;
            srcEdges.push_front(&*sink);

            if (!directed) {
                addEdge(to, from);
            }
        }

        return true;
    }

    bool addEdge(Vertex& from, Vertex& to) {
        return addEdge(from.name, to.name);
    }

    bool addEdge(Vertex& from, std::string to) {
        return addEdge(from.name, to);
    }

    bool addEdge(std::string from, Vertex& to) {
        return addEdge(from, to.name);
    }

    void print(std::ostream& out = std::cout) {
        out << "Printing Graph..." << std::endl;
        out << "Size: " << nvertices() << std::endl;
        for (auto& vec : *this) {
            vec.print(out);
        }
        out << std::endl;
    }

    static Graph::edge_t edgeClassification(Vertex& v, Vertex& w) {
        if (w.parent == &v) return Graph::TREE_EDGE;
        if (w.discovered && !w.processed) return Graph::BACK_EDGE;
        if (w.processed && w.entryTime > v.entryTime) return Graph::FORWARD_EDGE;
        if (w.processed && w.entryTime < v.entryTime) return Graph::CROSS_EDGE;
        // warn
        return Graph::UNCLASSIFIED_EDGE;
    }

    iterator begin() { return iterator(vertices.begin()); }
    iterator end()   { return iterator(vertices.end());   }

    size_t nvertices() { return vertices.size(); }
    size_t nedges() { return nEdges(); }
    const bool directed = false;

    /* search variables */
    // time counter represents hierarchy in dfs
    int time = 0;
    // user can set `finished` to terminate search early
    bool finished = false;
private:
    void dfs(Vertex* v,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&)> = nullptr,
             std::function<void (Vertex&, Vertex&)> = nullptr);

    std::unordered_map<std::string, Vertex*> vertices;
    int nEdges = 0;
};

/* algorithm definitions */
#include "../algorithms/Graphs/topsort.h"
#include "../algorithms/Graphs/bfs.h"
#include "../algorithms/Graphs/dfs.h"

#endif
