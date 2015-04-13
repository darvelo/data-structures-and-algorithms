#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <exception>
#include "../../data-structures/Graph.h"

struct Data {
    Data(std::string _surname = "")
        : surname(_surname) { }
    std::string surname;
};

struct VertexRelationship {
    std::string name;
    std::vector<std::string> edges;
};

class BadVertexException : public std::exception {
public:
    BadVertexException(std::string _name)
        : name(_name) { }
    ~BadVertexException() throw() { }
    virtual const char* what() const throw() {
        return ("Accessing vertex: \"" + name + "\" failed!").c_str();
    }

    std::string name;
};

class BadEdgeException : public std::exception {
public:
    BadEdgeException(std::string _source, std::string _sink)
        : source(_source), sink(_sink) { }
    ~BadEdgeException() throw() { }
    virtual const char* what() const throw() {
        return ("Adding edge: \"" + source + " -> " + sink + "\" failed!").c_str();
    }

    std::string source;
    std::string sink;
};

// format for lines in the inputfile:
//
//     vertexName vertexWeight edge...
//
// vertexName   => string
// vertexWeight => int
// edge         => vertexName
//
// example:
//     v1 10 v2
//     v2 1 v3
//     v3 2
//
// creates three vertices: v1, v2, and v3. v1 points to v2 (v2 also points to v1 in
// an undirected graph), v2 points to v3, and v3 doesn't point to anything.
//
// NOTE: naming an edge vertex does not implicitly create that vertex in the graph.
//       an error will be thrown in this case. only vertices specified as the first
//       word on a line will be created.
static std::string vertex_string = "^([a-zA-Z0-9]+)\\s+([0-9]+)(?:\\s+([\\sa-zA-Z0-9]+))?\\s*$";
static std::regex vertex_regex(vertex_string);

template <typename Data>
static
void
addVertices(Graph<Data>& g, std::string filename, std::vector<VertexRelationship>& vertices) {
    std::ifstream file(filename);
    std::string line;
    std::smatch match;
    std::vector<std::string> empty;

    while (std::getline(file, line)) {
        if (line.empty() || line.size() == 1 || line[0] == '#') {
            continue;
        }

        if (std::regex_match(line, match, vertex_regex)) {
            std::string name = match[1];
            std::string weightStr = match[2];
            int weight = std::atoi(weightStr.c_str());

            g.addVertex(name, weight);

            std::stringstream edgeStrs(match[3]);
            std::istream_iterator<std::string> begin(edgeStrs);
            std::istream_iterator<std::string> end;
            vertices.push_back({ name, {begin, end} });
        }
    }

    file.close();
}

template <typename Data>
void
addEdges(Graph<Data>& g, std::vector<VertexRelationship>& vertices) {
    for (VertexRelationship& r : vertices) {
        auto& source = r.name;

        if (g.getVertex(source) == g.end()) {
            throw BadVertexException(source);
        }

        for (auto& sink : r.edges) {
            if (g.getVertex(sink) == g.end()) {
                throw BadVertexException(sink);
            }

            if(!g.addEdge(source, sink)) {
                throw BadEdgeException(source, sink);
            }
        }
    }
}

template <typename Data>
void
readIntoGraph(Graph<Data>& g, std::string filename) {
    std::vector<VertexRelationship> vertices;
    addVertices(g, filename, vertices);
    addEdges(g, vertices);
}

#endif
