#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include "../../data-structures/Graph.h"

struct Data {
    Data(std::string _surname = "") : surname(_surname) { }
    std::string surname;
};

struct VectorRelationship {
    std::string name;
    std::vector<std::string> edges;
};

// format: name weight edges...
static std::string vertex_string = "^([a-zA-Z0-9]+)\\s+([0-9]+)(?:\\s+([\\sa-zA-Z0-9]+))?\\s*$";
static std::regex vertex_regex(vertex_string);

template <typename Data>
void
readIntoGraph(Graph<Data>& g, std::string filename) {
    std::ifstream f(filename);
    std::string line;
    std::smatch match;
    std::vector<VectorRelationship> vectors;
    std::vector<std::string> empty;

    while (std::getline(f, line)) {
        if (line.empty() || line.size() == 1) {
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
            vectors.push_back({ name, {begin, end} });
        }
    }

    auto begin = vectors.begin();
    auto end = vectors.end();

    while (begin != end) {
        VectorRelationship& r = *begin;
        auto itr = g.getVertex(r.name);

        if (itr != g.end()) {
            auto v = *itr;
            auto edgesBegin = r.edges.begin();
            auto edgesEnd = r.edges.end();

            while (edgesBegin != edgesEnd) {
                g.addEdge(v.name, *edgesBegin++);
            }
        }

        ++begin;
    }

    f.close();
}

#endif
