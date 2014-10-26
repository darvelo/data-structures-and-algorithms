#ifndef SHELLSORT_H
#define SHELLSORT_H

#include <cstddef>
#include <cmath>
#include <vector>

template <typename Comparable>
void shellSort(std::vector<Comparable>& a) {
    // build magic gap sequence
    // according to _Data Structures and Algorithm Analysis in C++_ by Mark Allen Weiss,
    //   the best sequence is produced by the combination of these two formulas shown.
    // according to wikipedia, the best results in the average case are "when the ratios
    //   of successive gaps are roughly equal to 2.2", or 2.25 according to Tokuda
    std::vector<size_t> sequence;
    size_t gap = 1, i = 1;
    while (gap < a.size()) {
        // the first formula produces `-1` with `i=1`, but the second is OK.
        // thus `i` must be manually manipulated to keep the two formulas in successive sync.
        sequence.push_back(gap);
        gap = pow(4,++i) - 3 * pow(2,i) + 1;
        sequence.push_back(gap);
        gap = 9 * pow(4,--i) - 9 * pow(2,i++) + 1;
    }

    // begin sorting based on magic gap sequence, largest to smallest
    for (auto gap = sequence.rbegin(); gap != sequence.rend(); ++gap) {
        for (size_t i = *gap; i < a.size(); ++i) {
            Comparable tmp = std::move(a[i]);

            size_t j;

            for (j = i; j >= *gap && tmp < a[j - *gap]; j -= *gap) {
                a[j] = std::move(a[j - *gap]);
            }

            a[j] = std::move(tmp);
        }
    }
}

#endif
