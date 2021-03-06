/*
 * In this version, the shape of the tree is dependent on the order in which
 * the keys are inserted. Inserting in sorted order creates one skinny tree!
 */
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <functional> /* less, function */
#include <utility> /* move, forward */

template <typename Object, typename Comparator=std::less<Object>>
class BinarySearchTree {
private:
    struct Node {
        Object data;
        Node* left = nullptr;
        Node* right = nullptr;

        Node (const Object& x, Node* _left = nullptr, Node* _right = nullptr)
            : data(x), left(_left), right(_right) { }
        Node (Object&& x, Node* _left = nullptr, Node* _right = nullptr)
            : data(std::move(x)), left(_left), right(_right) { }
    };
public:
    BinarySearchTree() { }
    BinarySearchTree(const BinarySearchTree& rhs) : root{nullptr} {
        *this = rhs;
    }
    BinarySearchTree(BinarySearchTree&& rhs) : root{nullptr} {
        *this = std::forward<BinarySearchTree>(rhs);
    }

    BinarySearchTree& operator=(const BinarySearchTree& rhs) {
        root = clone(rhs.root);
    }
    BinarySearchTree& operator=(BinarySearchTree&& rhs) {
        clear();
        root = std::move(rhs.root);
        rhs.root = nullptr;
    }

    ~BinarySearchTree() {
        clear();
    }

    Node* min () const {
        min(root);
    }

    Node* min (Node* min) const {
        if (min == nullptr) {
            return nullptr;
        }

        while (min->left != nullptr) {
            min = min->left;
        }

        return min;
    }

    Node* max () const {
        max(root);
    }

    Node* max (Node* max) const {
        if (max == nullptr) {
            return nullptr;
        }

        while (max->right != nullptr) {
            max = max->right;
        }

        return max;
    }

    Node* search (const Object& data) const {
        return search(data, root);
    }

    bool contains(const Object& data) const {
        return search(data, root) != nullptr;
    }

    void insert (const Object& data) {
        insert(data, root);
    }

    void insert (Object&& data) {
        insert(std::forward<Object>(data), root);
    }

    void remove (const Object& data) {
        remove(data, root);
    }

    void clear () {
        clear(root);
    }

    void print (std::ostream& out = std::cout) {
        print(root, out);
    }
private:
    Node* search (const Object& data, Node*const& t) const {
        if (t == nullptr || data == t->data) {
            return t;
        }

        if (isLessThan(t->data, data)) {
            return search(data, t->right);
        } else {
            return search(data, t->left);
        }
    }

    void insert (const Object& data, Node*& t) {
        if (t == nullptr) {
            t = new Node(data);
        } else if (isLessThan(data, t->data)) {
            insert(data, t->left);
        } else if (isLessThan(t->data, data)) {
            insert(data, t->right);
        } else {
            ; // duplicate data - do nothing
        }
    }

    void insert (Object&& data, Node*& t) {
        if (t == nullptr) {
            t = new Node(std::move(data));
        } else if (isLessThan(data, t->data)) {
            insert(std::forward<Object>(data), t->left);
        } else if (isLessThan(t->data, data)) {
            insert(std::forward<Object>(data), t->right);
        } else {
            ; // duplicate data - do nothing
        }
    }

    void remove (const Object& data, Node*& t) {
        if (t == nullptr) {
            return;
        } else if (isLessThan(data, t->data)) {
            remove(data, t->left);
        } else if (isLessThan(t->data, data)) {
            remove(data, t->right);
        } else if (t->left != nullptr && t->right != nullptr) {
            // two child nodes
            t->data = min(t->right)->data;
            remove(t->data, t->right);
        } else  {
            // zero or one child node
            Node* oldNode = t;
            t = (t->right != nullptr) ? t->right : t->left;
            delete oldNode;
        }
    }

    void clear (Node*& t) {
        if (t != nullptr) {
            clear(t->left);
            clear(t->right);
            delete t;
            t = nullptr;
        }
    }

    Node* clone (Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        return new Node(t->data, clone(t->left), clone(t->right));
    }

    void print (Node* t, std::ostream& out = std::cout) {
        preOrderTraversal(t, [&out] (Node* t) {
            out << "data: " << t->data;

            out << ", left: ";
            (t->left  != nullptr) ? out << t->left->data  : out << "null";

            out << ", right: ";
            (t->right != nullptr) ? out << t->right->data : out << "null";

            out << std::endl;
        });
        out << std::endl;
    }

    void preOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullptr) {
            return;
        }

        processNode(t);
        preOrderTraversal(t->left, processNode);
        preOrderTraversal(t->right, processNode);
    }

    void inOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullptr) {
            return;
        }

        inOrderTraversal(t->left, processNode);
        processNode(t);
        inOrderTraversal(t->right, processNode);
    }

    void postOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullptr) {
            return;
        }

        postOrderTraversal(t->left, processNode);
        postOrderTraversal(t->right, processNode);
        processNode(t);
    }

    Comparator isLessThan;
    Node* root = nullptr;
};

#endif
