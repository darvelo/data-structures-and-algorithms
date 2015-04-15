#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H

#include <unordered_map>

template <typename Key>
class DisjointSets {
public:
    DisjointSets() { }

    Key find(Key x) {
        if (parent.find(x) == parent.end()) {
            return x;
        } else {
            // path compression
            return parent[x] = find(parent[x]);
        }
    }

    void unionSets(Key s1, Key s2) {
        Key root1 = find(s1);
        Key root2 = find(s2);

        // initialize setSizes as needed
        if (!setSize[root1]) setSize[root1] = 1;
        if (!setSize[root2]) setSize[root2] = 1;

        if (root1 == root2) return;

        // if root1 holds more elements
        if (setSize[root1] >= setSize[root2]) {
            setSize[root1] = setSize[root1] + setSize[root2];
            parent[root2] = root1;
        } else {
            setSize[root2] = setSize[root1] + setSize[root2];
            parent[root1] = root2;
        }
    }

    bool sameRoot(Key s1, Key s2) {
        return find(s1) == find(s2);
    }

    size_t size() { return parent.size(); }
private:
    std::unordered_map<Key, Key> parent;
    std::unordered_map<Key, int> setSize;
};

#endif
