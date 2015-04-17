#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <limits>
#include "../../../data-structures/Matrix.h"
#include "../helpers.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::tuple;
using std::get;

static int MAX_INT = std::numeric_limits<int>::max();

void allPairs(const Matrix<int>& adj,
                    Matrix<int>& dist,
                    Matrix<int>& next)
{
    int n = adj.nrows();
    int distance;

    // initialize dist and path
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            distance = dist[i][j] = adj[i][j];
            next[i][j] = (distance == MAX_INT) ? -1 : j;
        }
    }

    // for all intermediate vertices
    for (int k = 0; k < n; ++k) {
        // consider each vertex through the intermediate
        for (int i = 0; i < n; ++i) {
            // skip if no path from start to intermediate vertex
            if (dist[i][k] == MAX_INT) continue;

            for (int j = 0; j < n; ++j) {
                // skip if no path from intermediate vertex to destination
                if (dist[k][j] == MAX_INT) continue;

                // we're assured we're not adding MAX_INT and overflowing here
                int through_k = dist[i][k] + dist[k][j];

                // if the path directly from i to j is longer than through k
                if (dist[i][j] > through_k) {
                    // update shortest path
                    dist[i][j] = through_k;
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

template <typename GraphT>
int
vertexNameToInt(GraphT& g,
                string name,
                unordered_map<typename GraphT::Vertex*, int>& vtoi)
{
    auto iter = g.getVertex(name);

    if (iter == g.end()) {
        throw BadVertexException(name);
    }

    return vtoi[&*iter];
}

template <typename Vertex>
vector<string>
getPath(Matrix<int>& next,
        string start,
        int i,
        int j,
        unordered_map<int, Vertex*>& itov)
{
    vector<string>result;

    if (next[i][j] == -1) {
        result.push_back("NO PATH");
        return result;
    }

    result.push_back(start);

    while (i != j) {
        i = next[i][j];
        result.push_back(itov[i]->name);
    }

    return result;
}

int main(int argc, char* argv[]) {
    bool directed = true;
    Graph<Data> g(directed);
    readIntoGraph(g, "./input/spanning_tree_graph.txt");

    tuple<unordered_map<int, Graph<Data>::Vertex*>,
          unordered_map<Graph<Data>::Vertex*, int>,
          Matrix<int>>
        tup = graphToMatrix(g);

    auto& itov = get<0>(tup);
    auto& vtoi = get<1>(tup);
    auto& adj  = get<2>(tup);

    int n = g.nvertices();
    Matrix<int> dist(n, n);
    Matrix<int> next(n, n);

    for (auto pr : itov) {
        cout << pr.first << ": " << pr.second->name << endl;
    }
    cout << endl;

    allPairs(adj, dist, next);
    cout << "Adjacency Matrix:" << endl << adj;
    cout << "Distance Matrix:"  << endl << dist;
    cout << "Path Matrix:"      << endl << next;

    // print path between labeled vertices in arguments
    if (argc == 3) {
        int i = vertexNameToInt(g, argv[1], vtoi);
        int j = vertexNameToInt(g, argv[2], vtoi);
        vector<string> path = getPath(next, argv[1], i, j, itov);

        if (!path.empty()) {
            cout << path[0];
        }
        for (decltype(path.size()) i = 1; i < path.size(); ++i) {
            cout << " -> ";
            cout << path[i];
        }
        cout << " *** DISTANCE: " << dist[i][j] << endl;
    }

    return 0;
}
