#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>
#include <unordered_set>
#include <limits>
#include "../../../data-structures/Graph.h"
#include "../helpers.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::multiset;
using std::unordered_set;

const int MAX_INT = std::numeric_limits<int>::max();

template <typename Vertex>
struct f_score_type {
    f_score_type(Vertex* _v, int _score)
        : v(_v), f_score(_score) { }

    Vertex* v;
    int f_score;
};

template <typename f_score_type>
struct fscoreCompare {
    fscoreCompare() { }
    bool operator()(f_score_type a, f_score_type b) const {
        return a.f_score < b.f_score;
    }
};

template <typename Vertex>
int heuristic_cost_estimate(Vertex* v, Vertex* goal) {
    // get distance between vertex coordinates using Pythagorean Theorem
    return v->data->distFrom(goal->data);
}

template <typename GraphT, typename Vertex>
bool
A_star(GraphT& g, Vertex& start, Vertex& goal) {
    typedef f_score_type<Vertex> f_type;
    // set of tentative nodes to be evaluated, sorted by f_score ascending
    multiset<f_type, fscoreCompare<f_type>> openset;
    // set of nodes already evaluated
    unordered_set<Vertex*> closedset;

    g.initializeSearch();
    start.distance = 0;
    // estimated total cost from start to goal through y
    openset.emplace(&start, start.distance + heuristic_cost_estimate(&start, &goal));

    auto vertexInOpenset = [&openset] (Vertex* w) {
        for (decltype(openset.begin()) beg = openset.begin(); beg != openset.end(); ++beg) {
            if ((*beg).v == w) return beg;
        }

        return openset.end();
    };

    while (!openset.empty()) {
        f_type f = *openset.begin();
        Vertex* v = f.v;

        if (v == &goal) return true;

        cout << "REMOVING " << v->name << " FROM OPENSET. F_SCORE: " << f.f_score << endl;

        openset.erase(openset.begin());
        closedset.insert(v);

        for (auto& e : v->edges) {
            auto w = e.w;

            if (closedset.find(w) != closedset.end()) continue;

            // prevent overflowing distance int while adding paths
            int dist = MAX_INT;
            if (v->distance != MAX_INT && e.weight != MAX_INT) {
                dist = v->distance + e.weight;
            }

            auto iter = vertexInOpenset(w);

            // update w's distance if the path through v is shorter
            if (iter == openset.end() || dist < w->distance) {
                w->parent = v;
                w->distance = dist;

                // prepare to update f_score by removing
                // and reinserting w with the new f_score
                if (iter != openset.end()) {
                    openset.erase(iter);
                }

                // if openset doesn't contain w, add it along with its f_score
                openset.emplace(w, dist + heuristic_cost_estimate(w, &goal));
            }
        }
    }

    return false;
}

template <typename Vertex>
void
getPath(Vertex goal, vector<string>& result) {
    if (goal->parent) {
        getPath(goal->parent, result);
    }
    result.push_back(goal->name);
}

template <typename Vertex>
void tracePath(Vertex goal, vector<vector<char>>& mapMatrix) {
    auto x = goal->data->x;
    auto y = goal->data->y;

    mapMatrix[x][y] = '@';

    if (goal->parent) {
        tracePath(goal->parent, mapMatrix);
    }
}

void printMap(vector<vector<char>>& mapMatrix) {
    decltype(mapMatrix.size()) width = mapMatrix.size();
    decltype(width) height = mapMatrix[0].size();

    for (decltype(height) y = 0; y < height; ++y) {
        for (decltype(width) x = 0; x < width; ++x) {
            cout << mapMatrix[x][y];
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " inputfile startCoords goalCoords [allowDiagonals] [diagonalCost]" << endl;
        cout << "e.g.: " << argv[0] << " input.txt \"(10,7)\" \"(3,7)\" y 2" << endl;
        cout << "e.g.: " << argv[0] << " input.txt \"(10,7)\" \"(3,7)\"" << endl;
        cout << "Diagonals are disallowed by default." << endl;
        cout << "Non-diagonals cost 1 unit. Diagonals cost 1 unit by default." << endl;
        return 1;
    }

    Graph<Point> g;
    vector<vector<char>> mapMatrix;
    bool allowDiagonals = false;
    int diagonalCost = 1;

    if (argc > 4) {
        if (strcmp(argv[4], "y") == 0) allowDiagonals = true;
        if (strcmp(argv[4], "n") == 0) allowDiagonals = false;
    }

    if (argc > 5) {
        diagonalCost = std::atoi(argv[5]);
    }

    readMapIntoGraph(g, argv[1], allowDiagonals, diagonalCost, &mapMatrix);

    auto start = g.getVertex(argv[2]);
    auto goal = g.getVertex(argv[3]);

    if (start == g.end()) {
        cout << "Start vertex was invalid." << endl;
        return 2;
    }
    if (goal == g.end()) {
        cout << "Goal vertex was invalid." << endl;
        return 3;
    }

    if ((*start).data->blocked()) {
        cout << "Start vertex was not traversable." << endl;
        return 4;
    }
    if ((*goal).data->blocked()) {
        cout << "Goal vertex was not traversable." << endl;
        return 5;
    }

    vector<string> path;
    bool result = A_star(g, *start, *goal);

    if (result) {
        getPath(&*goal, path);

        if (!path.empty()) {
            cout << "PATH IS: " << path[0];
        }
        for (decltype(path.size()) i = 1; i < path.size(); ++i) {
            cout << " -> " << path[i];
        }
        cout << " *** DISTANCE: " << (*goal).distance << endl;

        printMap(mapMatrix);
        tracePath(&*goal, mapMatrix);
        printMap(mapMatrix);
    } else {
        cout << "PATH NOT FOUND" << endl;
    }

    return 0;
}
