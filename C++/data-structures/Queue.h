#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

template <typename Object>
class Queue {
public:
    void enqueue(const Object& element) {
        lst.push_front(element);
    }

    void enqueue(const Object&& element) {
        lst.push_front(std::move(element));
    }

    Object dequeue() {
        Object element = lst.back();
        lst.pop_back();
        return element;
    }

private:
    List<Object> lst;
};

#endif
