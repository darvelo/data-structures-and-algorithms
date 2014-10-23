#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include <cstddef>
#include <vector>

using std::move;
using std::vector;

template <typename Comparable>
void insertionSort(vector<Comparable>& a) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        Comparable tmp = std::move(a[i]);

        std::size_t j;

        for (j = i; j > 0 && tmp < a[j - 1]; --j) {
            a[j] = std::move(a[j - 1]);
        }

        a[j] = std::move(tmp);
    }
}

template <typename Iterator>
void insertionSort(Iterator begin, Iterator end) {
    if (begin == end) {
        return;
    }

    Iterator j;

    for (Iterator i = begin + 1; i != end; ++i) {
        auto tmp = std::move(*i);

        for (j = i; j != begin && tmp < *(j - 1); --j) {
            *j = std::move(*(j - 1));
        }

        *j = std::move(tmp);
    }
}

#endif
