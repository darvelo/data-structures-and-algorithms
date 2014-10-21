#ifndef LIST_H
#define LIST_H

#include <algorithm>

using std::move;
using std::swap;

template <typename Object>
class List {
private:
    struct Node {
        Object data;
        Node* next;
        Node* prev;

        Node(const Object & obj = Object(), Node* p = nullptr, Node* n = nullptr)
            : data{obj}, prev{p}, next{n} { }

        Node(Object && obj, Node* p = nullptr, Node* n = nullptr)
            : data{std::move(obj)}, prev{p}, next{n} { }
    };

public:
    class const_iterator {
    public:
        const_iterator() : current{nullptr} { }

        const Object & operator* () {
            return retrieve();
        }

        const_iterator & operator++ () {
            current = current->next;
            return *this;
        }

        const_iterator operator++ (int) {
            const_iterator old = *this;
            ++( *this );
            return old;
        }

        const_iterator & operator-- () {
            current = current->prev;
            return *this;
        }

        const_iterator operator-- (int) {
            const_iterator old = *this;
            --( *this );
            return old;
        }

        bool operator== (const const_iterator& rhs) const {
            return this->current == rhs.current;
        }

        bool operator!= (const const_iterator& rhs) const {
            return !(*this == rhs);
        }
    protected:
        Object & retrieve() { return current->data; }

        const_iterator(Node* p)
            : current{p} { }

        Node* current = nullptr;

        friend List<Object>;
    };

    class iterator : public const_iterator {
    public:
        iterator() : const_iterator() { }

        Object & operator* () {
            return const_iterator::retrieve();
        }

        const Object & operator* () const {
            return const_iterator::operator*;
        }

        iterator & operator++ () {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++ (int) {
            iterator old = *this;
            ++( *this );
            return *this;
        }

        iterator& operator-- () {
            this->current = this->current->prev;
            return *this;
        }

        iterator operator-- (int) {
            iterator old = *this;
            --( *this );
            return old;
        }
    protected:
        iterator(Node* p) : const_iterator{p} { }

        friend List<Object>;
    };

    List(size_t initialSize = 0, Object initialValue = Object()) {
        init();

        for (size_t i = 0; i != initialSize; ++i) {
            push_back(initialValue);
        }
    }

    ~List() {
        clear();
        delete head;
        delete tail;
    }

    size_t size() {
        return theSize;
    }

    bool empty() {
        return size() == 0;
    }

    iterator insert(iterator iter, const Object& data) {
        Node* node = new Node(data, (iter.current)->prev, (iter.current));

        (iter.current)->prev->next = (iter.current)->prev = node;

        ++theSize;

        return iterator(node);
    }

    iterator insert(iterator iter, const Object&& data) {
        Node* node = new Node(std::move(data), (iter.current)->prev, (iter.current));

        (iter.current)->prev->next = (iter.current)->prev = node;

        ++theSize;

        return iterator(node);
    }

    iterator erase(iterator iter) {
        iterator ret{(iter.current)->next};

        (iter.current)->prev->next = (iter.current)->next;
        (iter.current)->next->prev = (iter.current)->prev;

        delete iter.current;

        --theSize;

        return ret;
    }

    List& clear() {
        while (!empty()) {
            pop_front();
        }

        return *this;
    }

    void push_front(const Object& data) {
        insert(begin(), data);
    }

    void push_back(const Object& data) {
        insert(end(), data);
    }

    List& pop_front() {
        erase(begin());
        return *this;
    }

    List& pop_back() {
        erase(--end());
        return *this;
    }

    Object & back() { return tail->prev->data; }
    const Object& back() const { return tail->prev->data; }

    Object & front() { return head->next->data; }
    const Object& front() const { return head->next->data; }

    iterator begin() { return iterator(head->next); }
    const_iterator begin() const { return const_iterator(head->next); }

    iterator end() { return iterator(tail); }
    const_iterator end() const { return const_iterator(tail); }

    void init() {
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
    }
private:
    size_t theSize = 0;
    Node* head;
    Node* tail;
};

#endif
