#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <regex>
#include "../../data-structures/Graph.h"
#include "error_types.h"

// Sample user-defined data
struct Data {
    Data(std::string _surname = "")
        : surname(_surname) { }
    std::string surname;
};

// format for lines in the inputfile:
//
//     vertexName [vertexWeight edge...]
//
// vertexName   => string
// vertexWeight => int
// edge         => vertexName
//
// example:
//     v1 1 v2 v3
//     v1 8 v4
//     v2
//     v3 10 v2
//     v4 4 v1
//
// creates four vertices: v1, v2, v3, and v4.
// v1 points to v2 and v3; each edge has a weight of 1.
// v2 and v3 also point to v1 with edge weight 1 in an undirected graph.
// v1 also points to v4, with an edge weight of 8.
// v2 doesn't point to anything unless the graph is undirected.
// v3 points to v2 with edge weight 10,
// and v4 points to v1 with edge weight 4.
//
// lines can be reordered without trouble.
// comments are lines that start with a '#'.
// whitespace is allowed anywhere but the beginning of a line. blank lines are OK.
//
// NOTE: Errors are thrown in the following cases:
//       *  naming an edge vertex that is not defined elsewhere on its own line as the first word.
//       *  defining an edge that was previously defined
//       *  defining edges on two vertices with different weights on an undirected graph.
//       *  if for some reason adding an edge to the graph does not work
std::string vertex_string = "^([a-zA-Z0-9]+)(?:\\s+([0-9]+)\\s+([\\sa-zA-Z0-9]+))?\\s*$";
std::regex vertex_regex(vertex_string);

void readData(std::string filename, std::unordered_map<std::string,std::unordered_map<std::string,int>>& relationships) {
    std::ifstream file(filename);
    std::string line;
    std::smatch match;
    std::vector<std::string> empty;
    std::unordered_map<std::string, int> dummy;

    while (std::getline(file, line)) {
        if (line.empty() || line == "\n" || line[0] == '#') {
            continue;
        }

        if (!std::regex_match(line, match, vertex_regex)) {
            continue;
        }

        // get name
        std::string name = match[1];

        // test if vertex has no edges
        if (match[2] == "") {
            relationships.insert({ name, dummy });
            continue;
        }

        // get weight
        std::string weightStr = match[2];
        int weight = std::atoi(weightStr.c_str());

        // get edge vertex names
        std::stringstream edgeStrs(match[3]);

        // push edges into vertex's edge map
        std::istream_iterator<std::string> begin(edgeStrs);
        std::istream_iterator<std::string> end;
        std::string edgeName;
        auto& edgeMap = relationships[name];

        while (begin != end) {
            edgeName = *begin;

            if (edgeMap.find(edgeName) != edgeMap.end()) {
                throw EdgeReassignmentException(name, edgeName, edgeMap[edgeName], weight);
            }

            relationships[name][edgeName] = weight;
            ++begin;
        }
    }

    file.close();
}

template <typename GraphT>
void
populateGraph(GraphT& g, std::unordered_map<std::string, std::unordered_map<std::string,int>>& relationships) {
    // add all vertices first
    for (auto& vertexMap : relationships) {
        g.addVertex(vertexMap.first);
    }

    // now connect them with edges
    for (auto& vertexMap : relationships) {
        auto& source = vertexMap.first;

        for (auto& edgeMap : vertexMap.second) {
            auto& sink = edgeMap.first;
            int weight = edgeMap.second;

            if (g.getVertex(sink) == g.end()) {
                throw BadVertexException(sink);
            }

            auto reverseEdge = relationships[sink].find(source);

            if (!g.directed &&
                reverseEdge != relationships[sink].end() &&
                reverseEdge->second != weight)
            {
                throw UndirectedGraphEdgeException(source, sink, reverseEdge->second, weight);
            }

            if(!g.addEdge(source, sink, weight)) {
                throw BadEdgeException(source, sink);
            }
        }
    }
}

template <typename GraphT>
void
readIntoGraph(GraphT& g, std::string filename) {
    std::unordered_map<std::string, std::unordered_map<std::string,int>> relationships;
    readData(filename, relationships);
    populateGraph(g, relationships);
}

#endif
