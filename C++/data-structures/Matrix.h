#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

template <typename Object>
class Matrix {
public:
    using size_type = typename std::vector<Object>::size_type;
    using iterator = typename std::vector<std::vector<Object>>::iterator;
    using const_iterator = typename std::vector<std::vector<Object>>::const_iterator;

    Matrix(size_type rows, size_type cols) {
        std::vector<std::vector<Object>> tmp(rows, std::vector<Object>(cols));
        std::swap(array, tmp);
    }

    Matrix(size_type rows, size_type cols, Object initialVal) {
        std::vector<std::vector<Object>> tmp(rows, std::vector<Object>(cols, initialVal));
        std::swap(array, tmp);
    }

    Matrix(std::vector<std::vector<Object>> v)
        : array{v} { }
    Matrix(std::vector<std::vector<Object>>&& v)
        : array{std::move(v)} { }

    const std::vector<Object>& operator[](size_type row) const {
        return array[row];
    }

    std::vector<Object>& operator[](size_type row) {
        return array[row];
    }

    void print(std::ostream& out = std::cout) const {
        for (auto& row : *this) {
            for (int cell : row) {
                out << cell << " ";
            }
            out << std::endl;
        }
        out << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix<Object>& m) {
        m.print(out);
        return out;
    }

    iterator begin() { return array.begin(); }
    iterator end() { return array.end(); }
    const_iterator begin() const { return array.cbegin(); }
    const_iterator end() const { return array.cend(); }

    const size_type nrows() const { return array.size(); }
    const size_type ncols() const { return nrows() ? array[0].size() : 0; }
private:
    std::vector<std::vector<Object>> array;
};

#endif
