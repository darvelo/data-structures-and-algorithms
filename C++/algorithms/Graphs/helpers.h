#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <unordered_map>
#include <tuple>
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>
#include "../../data-structures/Graph.h"
#include "../../data-structures/Matrix.h"
#include "error_types.h"

// Sample user-defined data
struct Data {
    Data(std::string _surname = "")
        : surname(_surname) { }
    std::string surname;
};

struct Point {
    Point(int _x, int _y, char _type)
        : x(_x), y(_y), type(_type) { }

    bool blocked() const { return type == '^'; }

    // get distance between points using Pythagorean Theorem
    int distFrom(Point* goal) {
        int dx = abs(goal->x - x);
        int dy = abs(goal->y - y);
        return sqrt(pow(dx, 2) + pow(dy, 2));
    }

    const int x;
    const int y;
    char type;

    static std::string getNameOf(int x, int y) {
        std::stringstream s;
        s << "(" << x << "," << y << ")";
        return s.str();
    }
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

template <typename Data>
void
createMapEdges(Graph<Data>& g, int x1, int y1, int x2, int y2) {
    std::string from = Data::getNameOf(x1, y1);
    Data* current = (*g.getVertex(from)).data;

    if (current->blocked()) {
        return;
    }

    std::string to = Data::getNameOf(x2, y2);
    Data* adjacent = (*g.getVertex(to)).data;

    if (adjacent->blocked()) {
        return;
    }

    g.addEdge(from, to);
    g.addEdge(to, from);
}

template <typename Data>
void
readMapIntoGraph(Graph<Data>& g, std::string filename, bool allowDiagonals = true, std::vector<std::vector<char>>* mapMatrix = nullptr) {
    std::ifstream file(filename);
    std::string line;
    std::string::size_type width = 0;
    std::string::size_type height = 0;
    std::string::size_type& y = height;

    // add vertices
    while (std::getline(file, line)) {
        if (line.empty() || line == "\n" || line[0] == '#') {
            continue;
        }

        if (width == 0) {
            width = line.size();
        }

        if (line.size() != width) {
            throw CustomException("Matrix input graph must be rectangular.");
        }

        if (mapMatrix) {
            mapMatrix->resize(width);
            for (auto& c : *mapMatrix) {
                // make sure we have space starting from height = 0,
                // since height is only incremented at the end of the loop,
                // after the coordinate position is filled.
                c.resize(height+1);
            }
        }

        for (std::string::size_type x = 0; x < line.size(); ++x) {
            if (mapMatrix) (*mapMatrix)[x][y] = line[x];
            Data* data = new Data(x, y, line[x]);
            g.addVertex(Data::getNameOf(x, y), data);
        }

        ++height;
    }

    file.close();

    // add edges between unblocked territory
    for (std::string::size_type x = 0; x < width; ++x) {
        for (std::string::size_type y = 0; y < height; ++y) {
            // add horizontal edges
            if (x != 0) {
                createMapEdges(g, x, y, x-1, y);
            }

            // add vertical edges
            if (y != 0) {
                createMapEdges(g, x, y, x, y-1);
            }

            // add diagonals
            if (allowDiagonals && x != 0) {
                // NW/SE edges
                if (y != 0) {
                    createMapEdges(g, x, y, x-1, y-1);
                }
                // NE/SW edges
                if (y < height-1) {
                    createMapEdges(g, x, y, x-1, y+1);
                }
            }
        }
    }
}

template <typename GraphT>
std::tuple<std::unordered_map<int, typename GraphT::Vertex*>,
           std::unordered_map<typename GraphT::Vertex*, int>,
           Matrix<int>>
graphToMatrix(GraphT& g) {
    int MAX_INT = std::numeric_limits<int>::max();
    std::unordered_map<int, typename GraphT::Vertex*> itov;
    std::unordered_map<typename GraphT::Vertex*, int> vtoi;
    Matrix<int> m(g.nvertices(), g.nvertices(), MAX_INT);

    int count = 0;
    // map vertices to integers and vice-versa
    for (auto& v : g) {
        itov[count] = &v;
        vtoi[&v] = count;
        ++count;
    }

    // map edges into adjacency matrix
    for (int i = 0; i < count; ++i) {
        auto v = itov[i];
        for (auto& e : v->edges) {
            int j = vtoi[e.w];
            m[i][j] = e.weight;
        }
        // zero distance from a vertex to itself
        m[i][i] = 0;
    }

    return std::tuple<std::unordered_map<int, typename GraphT::Vertex*>,
                      std::unordered_map<typename GraphT::Vertex*, int>,
                      Matrix<int>>
                (std::move(itov), std::move(vtoi), std::move(m));
}

#endif
