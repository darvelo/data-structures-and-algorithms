#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include <cstddef>
#include <vector>

template <typename Comparable>
void insertionSort(std::vector<Comparable>& a) {
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
    insertionSort(begin, end, [] (decltype(*begin) a, decltype(*begin) b) -> bool {
        return a < b;
    });
}

template <typename Iterator, typename Comparator>
void insertionSort(Iterator begin, Iterator end, Comparator lessThan) {
    if (begin == end) {
        return;
    }

    Iterator j;

    for (Iterator i = begin + 1; i != end; ++i) {
        auto tmp = std::move(*i);

        for (j = i; j != begin && lessThan(tmp, *(j - 1)); --j) {
            *j = std::move(*(j - 1));
        }

        *j = std::move(tmp);
    }
}

#endif
