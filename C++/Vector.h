#ifndef VECTOR_H
#define VECTOR_H

/**
 * Implementation of vector
 */
#include <algorithm>

using std::move;
using std::swap;

template <typename Object>
class Vector {
public:
  explicit Vector(const size_t initialSize = 0)
    : _size{initialSize}, _capacity{initialSize + PADDING}
  {
    objects = new Object[_capacity];
  }

  Vector(const Vector<Object> & rhs)
    : _size{rhs._size}, _capacity{rhs._capacity}, objects{nullptr}
  {
     objects = new Object[_capacity];

     for (size_t i = 0; i != rhs.size(); ++i) {
       objects[i] = rhs.objects[i];
      }
  }

  Vector(Vector<Object> && rhs)
    : _size(rhs._size), _capacity(rhs._capacity), objects(rhs.objects)
  {
    rhs.objects = nullptr;
    rhs._size = 0;
    rhs._capacity = 0;
  }

  Vector<Object> operator=(const Vector<Object> & rhs) {
    Vector<Object> copy = rhs;
    std::swap(*this, copy);
    return *this;
  }

  ~Vector() {
    delete[] objects;
  }

  Vector<Object> & operator=(Vector<Object> && rhs) {
    std::swap(_size, rhs._size);
    std::swap(_capacity, rhs._capacity);
    std::swap(objects, rhs.objects);

    return *this;
  }

  size_t capacity() const {
    return _capacity;
  }

  size_t size() const {
    return _size;
  }

  bool empty() const {
    return size() == 0;
  }

  void reserve(size_t newCapacity) {
    if (newCapacity < _size) {
      return;
    }

    Object* newObjects = new Object[newCapacity];

    for (size_t i = 0; i != _size; ++i) {
      newObjects[i] =  std::move(objects[i]);
    }

    _capacity = newCapacity;
    std::swap(objects, newObjects);

    delete[] newObjects;
  }

  Vector<Object> & resize(size_t newSize) {
    if (newSize > _capacity) {
      reserve(newSize * 2);
    }
    _size = newSize;
    return *this;
  }

  Object& operator[](size_t pos) {
    if (pos >= size()) {
      throw "Out of range!";
    }

    return objects[pos];
  }

  const Object& operator[](size_t pos) const {
    if (pos >= size()) {
      throw "Out of range!";
    }

    return objects[pos];
  }

  Object & back() {
    if (size() == 0) {
      throw "This Vector has no items!";
    }

    return objects[_size - 1];
  }

  const Object & back() const {
    if (size() == 0) {
      throw "This Vector has no items!";
    }

    return objects[_size - 1];
  }

  Vector<Object> & push_back(const Object & item) {
    if (_capacity == _size) {
      reserve(_capacity * 2 + 1);
    }

    objects[_size++] = item;

    return *this;
  }

  Vector<Object> & push_back(Object && item) {
    if (_capacity == _size) {
      reserve(_capacity * 2 + 1);
    }

    objects[_size++] = std::move(item);

    return *this;
  }

  void pop_back() {
    --_size;
  }

  typedef Object* iterator;
  typedef const Object* const_iterator;

  iterator begin() {
    return &objects[0];
  }

  const_iterator begin() const {
    return &objects[0];
  }

  iterator end() {
    return &objects[size()];
  }

  const_iterator end() const {
    return &objects[size()];
  }

  const static size_t PADDING = 16;

private:
  Object* objects = nullptr;
  size_t _size = 0;
  size_t _capacity = PADDING;
};

#endif
