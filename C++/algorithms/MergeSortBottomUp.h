#ifndef MERGESORT_BOTTOMUP_H
#define MERGESORT_BOTTOMUP_H

#include <cstddef>
#include <vector>

/***********************
 * Function Prototypes *
 ***********************/
template <typename Comparable>
void sort (std::vector<Comparable>* a, std::vector<Comparable>* b, size_t low, size_t high);

template <typename Comparable>
void merge_bottomup (std::vector<Comparable>& a, std::vector<Comparable>& b, size_t left, size_t middle, size_t right);

/******************
 * Implementation *
 ******************/
template <typename Comparable>
void mergeSort_bottomup (std::vector<Comparable>& a) {
    std::vector<Comparable> scratch(a.size());
    sort(&a, &scratch, 0, a.size());
}

template <typename Comparable>
void sort (std::vector<Comparable>* a, std::vector<Comparable>* b, size_t low, size_t high)
{
    std::vector<Comparable>* original = a;
    std::vector<Comparable>* scratch = b;
    std::vector<Comparable>* tmp;

    for (size_t p = 1; p < high; p <<= 1) {
        for (size_t i = 0; i < high; i += p << 1) {
            size_t left = i;
            size_t middle = (i + p) > high ? high : i + p;
            size_t right = (i + (p << 1)) > high ? high : i + (p << 1);
            merge_bottomup(*a, *b, left, middle, right);
        }

        tmp = a;
        a = b;
        b = tmp;
    }

    if (original != a) {
        for (size_t i = 0; i < high; ++i) {
            (*original)[i] = (*a)[i];
        }
    }
}

template <typename Comparable>
void merge_bottomup (std::vector<Comparable>& a, std::vector<Comparable>& b, size_t left, size_t middle, size_t right)
{
    size_t i = left;
    size_t j = middle;
    size_t k = left;

    while (k < right) {
        if (i < middle && (j >= right || a[i] <= a[j])) {
            b[k++] = a[i++];
        } else {
            b[k++] = a[j++];
        }
    }
}

#endif
