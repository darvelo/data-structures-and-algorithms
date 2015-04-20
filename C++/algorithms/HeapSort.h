#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <vector>

inline
int heapSortLeftChild(int i) {
    return i * 2 + 1;
}

template <typename Comparable>
void
heapSortPercolateDown(std::vector<Comparable>& a, int i, int n) {
    Comparable tmp = std::move(a[i]);
    int child;

    while (heapSortLeftChild(i) < n) {
        child = heapSortLeftChild(i);

        if (child + 1 != n && a[child] < a[child + 1]) {
            ++child;
        }

        if (tmp < a[child]) {
            a[i] = std::move(a[child]);
        } else {
            break;
        }

        i = child;
    }

    a[i] = std::move(tmp);
}

template <typename Comparable>
void
heapSort(std::vector<Comparable>& a) {
    // buildHeap
    for (int i = a.size() / 2 - 1; i >= 0; --i) {
        heapSortPercolateDown(a, i, a.size());
    }

    // deleteMax
    for (int j = a.size() - 1; j > 0; --j) {
        std::swap(a[0], a[j]);
        heapSortPercolateDown(a, 0, j);
    }
}

#endif
