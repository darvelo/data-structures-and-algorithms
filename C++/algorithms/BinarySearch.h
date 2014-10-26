#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <vector>

template <typename Comparable>
int binarySearch(const std::vector<Comparable>& a, const Comparable& x) {
    int low  = 0,
        high = a.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (x < a[mid]) {
            high = mid - 1;
        } else if (x > a[mid]) {
            low = mid + 1;
        } else {
            return mid;
        }
    }

    return -1;
}

#endif
