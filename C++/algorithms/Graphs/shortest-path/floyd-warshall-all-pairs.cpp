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
                    Matrix<int>& path)
{
    int n = adj.nrows();

    // initialize dist and path
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = adj[i][j];
            path[i][j] = -1;
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

                int through_k = dist[i][k] + dist[k][j];

                // if the path directly from i to j is longer than through k
                if (dist[i][j] > through_k) {
                    // update shortest path
                    dist[i][j] = through_k;
                    path[i][j] = k;
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
void
backward(Matrix<int>& path,
         vector<string>& vec,
         int i,
         int j,
         unordered_map<int, Vertex*>& itov)
{
    // even though the path is -1 (no intermediate vertex), we know i and j are directly connected,
    // since we know j is an intermediate vertex between our starting vertex i and our destination
    if (path[i][j] == -1) {
        vec.push_back(itov[i]->name);
        return;
    }
    // get to the first intermediate vertex after i between i and k
    backward(path, vec, i, path[i][j], itov);
    // push all intermediate vertices between i and k in order
    vec.push_back(itov[path[i][j]]->name);
}

template <typename Vertex>
void
forward(Matrix<int>& path,
        vector<string>& vec,
        int i,
        int j,
        unordered_map<int, Vertex*>& itov)
{
    // even though the path is -1 (no intermediate vertex), we know i and j are directly connected,
    // since we know i is an intermediate vertex between our starting vertex and our destination, j
    if (path[i][j] == -1) {
        vec.push_back(itov[j]->name);
        return;
    }
    // push all intermediate vertices between k and j in order
    vec.push_back(itov[path[i][j]]->name);
    // get to the first intermediate vertex before j between k and j
    forward(path, vec, path[i][j], j,itov);
}

template <typename Vertex>
void
recurse(Matrix<int>& path,
        vector<string>& vec,
        int i,
        int j,
        unordered_map<int, Vertex*>& itov)
{
    int k = path[i][j];
    // push all intermediate vertices between i and k
    backward(path, vec, i, k, itov);
    // push k
    vec.push_back(itov[k]->name);
    // push all intermediate vertices between k and j
    forward(path, vec, k, j, itov);
}

template <typename Vertex>
vector<string>
getPath(Matrix<int>& path,
        Matrix<int>& dist,
        int i,
        int j,
        unordered_map<int, Vertex*>& itov)
{
    vector<string>result;

    if (dist[i][j] == MAX_INT) {
        result.push_back("NO PATH");
    } else if (path[i][j] == -1) {
        result.push_back(itov[i]->name);
        result.push_back(itov[j]->name);
    } else {
        recurse(path, result, i, j, itov);
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
    Matrix<int> path(n, n);

    for (auto pr : itov) {
        cout << pr.first << ": " << pr.second->name << endl;
    }
    cout << endl;

    allPairs(adj, dist, path);
    cout << "Adjacency Matrix:" << endl << adj;
    cout << "Distance Matrix:"  << endl << dist;
    cout << "Path Matrix:"      << endl << path;

    // print path between labeled vertices in arguments
    if (argc == 3) {
        int i = vertexNameToInt(g, argv[1], vtoi);
        int j = vertexNameToInt(g, argv[2], vtoi);
        auto vec = getPath(path, dist, i, j, itov);

        if (!vec.empty()) {
            cout << vec[0];
        }
        for (decltype(vec.size()) i = 1; i < vec.size(); ++i) {
            cout << " -> ";
            cout << vec[i];
        }
        cout << " *** WEIGHT: " << dist[i][j] << endl;
    }

    return 0;
}
