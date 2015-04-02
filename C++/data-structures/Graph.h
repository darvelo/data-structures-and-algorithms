#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <iostream>
#include <algorithm>
#include <unordered_map>

template <typename Data>
class Graph {
public:
    class Vertex {
    public:
        void print(std::ostream& out = std::cout) {
            out << "Vertex: " << name << ", Weight: " << weight << ", Edges:" << std::endl;
            for (auto& v : edges) {
                out << "    \"" << v->name << "\" " << v->weight << std::endl;
            }
        }

        int weight = 0;
        const std::string name;
        // pointer itself should stay const
        const Data* data = nullptr;
    private:
        explicit Vertex(std::string _name, int _weight = 0, Data* _data = nullptr)
            : name(_name), weight(_weight), data(_data) { }

        std::list<Vertex*> edges;
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
    Graph(bool _directed = false) : directed(_directed) { }

    Vertex& addVertex(std::string name, int weight = 0, Data* data = nullptr) {
        Vertex* v = new Vertex{name, weight, data};
        std::pair<std::string, Vertex*> item{name, v};

        auto result = vertices.insert(item);

        // if insert failed, map already had such a key.
        // grab the element associated with that key
        if (!result.second) {
            delete v;
            v = (*result.first).second;
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
            srcEdges.push_back(&*sink);
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

    iterator begin() { return iterator(vertices.begin()); }
    iterator end()   { return iterator(vertices.end());   }

    size_t nvertices() { return vertices.size(); }
    size_t nedges() { return nEdges(); }
    const bool directed = false;
private:
    std::unordered_map<std::string, Vertex*> vertices;
    int nEdges = 0;
};

#endif
