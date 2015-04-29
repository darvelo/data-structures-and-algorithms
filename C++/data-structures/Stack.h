#ifndef STACK_H
#define STACK_H

#include <utility> /* move */
#include "Vector.h"
#include "../utils.h"

template <typename Object>
class Stack {
public:
    Stack& push(Object& element) {
        theArray.push_back(element);
        return *this;
    }

    Stack& push(Object&& element) {
        theArray.push_back(std::move(element));
        return *this;
    }

    Object pop() {
        if (theArray.empty()) {
            throw CustomException("Stack::pop(): The stack was empty -- cannot pop!");
        }

        Object element = theArray.back();
        theArray.pop_back();

        return element;
    }

    Object& top() {
        if (theArray.empty()) {
            throw CustomException("Stack::top(): The stack was empty -- cannot get top!");
        }

        return theArray.back();
    }

    const Object& top() const {
        if (theArray.empty()) {
            throw CustomException("Stack::top(): The stack was empty -- cannot get top!");
        }

        return theArray.back();
    }

    Stack& clear() {
        while (!theArray.empty()) {
            theArray.pop_back();
        }

        return *this;
    }

    bool empty() const {
        return theArray.empty();
    }
private:
    Vector<Object> theArray;
};

#endif
