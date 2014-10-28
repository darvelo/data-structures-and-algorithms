#ifndef MERGESORT_H
#define MERGESORT_H

#include <cstddef>
#include <vector>

/***********************
 * Function Prototypes *
 ***********************/
template <typename Comparable>
void mergeSort(std::vector<Comparable>& a);

template <typename Comparable>
void mergeSort(std::vector<Comparable>& a,
               std::vector<Comparable>& tmp, size_t left, size_t right);

template <typename Comparable>
void merge(std::vector<Comparable>& a,
           std::vector<Comparable>& tmp, size_t left, size_t right, size_t end);

/******************
 * Implementation *
 ******************/
template <typename Comparable>
void mergeSort(std::vector<Comparable>& a) {
    std::vector<Comparable> tmp(a.size());

    mergeSort(a, tmp, 0, a.size() - 1);
}

template <typename Comparable>
void mergeSort(std::vector<Comparable>& a,
        std::vector<Comparable>& tmp, size_t left, size_t right) {
    if (a.size() < 2 || left >= right) {
        return;
    }

    size_t center = left + (right - left) / 2;
    mergeSort(a, tmp, left, center);
    mergeSort(a, tmp, center + 1, right);
    merge(a, tmp, left, center + 1, right);
}

template <typename Comparable>
void merge(std::vector<Comparable>& a, std::vector<Comparable>& tmp, size_t left, size_t right, size_t end) {
    size_t nelems = end - left + 1;

    for (size_t i = left, j = right; left <= end; ++left) {
        if (i < right && (j > end || a[i] < a[j])) {
            tmp[left] = std::move(a[i++]);
        } else {
            tmp[left] = std::move(a[j++]);
        }
    }

    for (size_t i = 0; i < nelems; ++i, --end) {
        a[end] = std::move(tmp[end]);
    }
}

#endif
