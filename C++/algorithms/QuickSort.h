#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <cstddef>
#include <vector>
#include "InsertionSort.h"

template <typename Comparable>
void quickSort(std::vector<Comparable>& a);
template <typename Comparable>
void quickSort(std::vector<Comparable>& a, size_t begin, size_t end);
template <typename Comparable>
size_t median3 (std::vector<Comparable>& a, size_t begin, size_t end);

template <typename Comparable>
void quickSort(std::vector<Comparable>& a) {
    quickSort(a, 0, a.size() - 1);
}

template <typename Comparable>
void quickSort(std::vector<Comparable>& a, size_t begin, size_t end) {
    // insertionsort on small-sized arrays is more performant than recursion
    if (begin + 10 > end) {
        insertionSort(a, begin, end + 1);
        return;
    }

    // get the best candidate for a pivot using a median-of-three comparison
    size_t mid = median3(a, begin, end),
           i = begin,
           j = end - 1;

    // swap the pivot out to sort the rest of the elements relative to its value.
    // swap it to `end - 1` since we already know `a[end]` is larger than the pivot.
    std::swap(a[mid], a[end - 1]);

    Comparable& pivot = a[end - 1];

    while (1) {
        // leave items less than pivot on the left
        while (a[++i] < pivot);
        // leave items greater than pivot on the right
        while (pivot  < a[--j]);
        if (i < j) {
            std::swap(a[i], a[j]);
        } else {
            break;
        }
    }

    // put the pivot in its rightful place, after items less than it, and before items greater than it
    std::swap(a[i], a[end - 1]);

    quickSort(a, begin, i - 1);
    quickSort(a, i + 1, end);
}

template <typename Comparable>
size_t median3 (std::vector<Comparable>& a, size_t begin, size_t end) {
    size_t mid = begin + (end - begin) / 2;

    if (a[mid] < a[begin]) {
        std::swap(a[begin], a[mid]);
    }

    if (a[end] < a[begin]) {
        std::swap(a[begin], a[end]);
    }

    if (a[end] < a[mid]) {
        std::swap(a[end], a[mid]);
    }

    return mid;
}

#endif
