#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <iostream>
#include <unordered_map>

template <typename Data>
class Graph {
private:
    class Vertex {
    public:
        explicit Vertex(std::string _name, int _weight = 0, Data* _data = nullptr)
            : name(_name), weight(_weight), data(_data) { }

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
        std::list<Vertex*> edges;
        friend Graph<Data>;
    };
public:
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
        iterator(typename std::unordered_map<std::string,Vertex*>::iterator itr)
            : current(itr) { }

        typename std::unordered_map<std::string, Vertex*>::iterator current;
        friend Graph<Data>;
    };
public:
    Graph() { }

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

    void print(std::ostream& out = std::cout) {
        out << "Printing Graph..." << std::endl;
        out << "Size: " << size() << std::endl;
        for (auto& vec : *this) {
            vec.print(out);
        }
        out << std::endl;
    }

    size_t size()    { return vertices.size(); }
    iterator begin() { return iterator(vertices.begin()); }
    iterator end()   { return iterator(vertices.end());   }
private:
    std::unordered_map<std::string, Vertex*> vertices;
};

#endif
