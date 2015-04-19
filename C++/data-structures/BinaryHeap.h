#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>
#include <stdexcept>

template <typename Comparable>
class BinaryHeap {
public:
    explicit BinaryHeap(int capacity = 100)
        : array(capacity) { }
    explicit BinaryHeap(const std::vector<Comparable>& items)
        : array(items.size() + 10), currentSize{items.size()}
    {
        for (int i = 0; i < items.size(); ++i) {
            array[i + 1] = items[i];
        }

        buildHeap();
    }

    const Comparable& findMin() const {
        if (empty()) {
            throw std::underflow_error("Can't findMin from an empty heap!");
        }

        return array[1];
    }

    void insert(const Comparable& x) {
        if (currentSize == array.size() - 1) {
            array.resize(array.size() * 2);
        }

        int hole = ++currentSize;
        Comparable copy = x;

        array[0] = std::move(copy);

        // percolate up
        while (x < array[hole/2]) {
            array[hole] = std::move(array[hole/2]);
            hole /= 2;
        }

        array[hole] = std::move(array[0]);
    }

    void insert(Comparable&& x) {
        insert(x);
    }

    void deleteMin() {
        if (empty()) {
            throw std::underflow_error("Can't deleteMin from an empty heap!");
        }

        array[1] = std::move(array[currentSize--]);
        percolateDown(1);
    }

    void deleteMin(Comparable& minItem) {
        if (empty()) {
            throw std::underflow_error("Can't deleteMin from an empty heap!");
        }

        minItem = std::move(array[1]);
        array[1] = std::move(array[currentSize--]);
        percolateDown(1);
    }

    void clear() {
        while (!empty()) {
            deleteMin();
        }
    }

    int size () const { return currentSize; }
    bool empty() const { return currentSize == 0; }
private:
    int currentSize;
    std::vector<Comparable> array;

    void buildHeap() {
        for (int i = currentSize / 2; i > 0; --i) {
            percolateDown(i);
        }
    }

    void percolateDown(int hole) {
        Comparable tmp = std::move(array[hole]);
        int child;

        while (hole * 2 <= currentSize) {
            child = hole * 2;

            if (child != currentSize && array[child+1] < array[child]) {
                ++child;
            }

            if (array[child] < tmp) {
                array[hole] = std::move(array[child]);
            } else {
                break;
            }

            hole = child;
        }

        array[hole] = std::move(tmp);
    }
};

#endif
