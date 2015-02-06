/*
 * In this version, the shape of the tree is dependent on the order in which
 * the keys are inserted. Inserting in sorted order creates one skinny tree!
 */
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>

template <typename Comparable>
class BinarySearchTree {
private:
    struct Node {
        Comparable data;
        Node* left = nullptr;
        Node* right = nullptr;

        Node (Comparable x) : data(x) { }
    };
public:
    BinarySearchTree() { }

    ~BinarySearchTree() {
        postOrderTraversal(root, [] (Node* t) {
            delete t->left;
            delete t->right;
            t->left = t->right = nullptr;
        });

        delete root;
        root = nullptr;
    }

    Node* min () {
        min(root);
    }

    Node* min (Node* min) {
        if (min == nullptr) {
            return nullptr;
        }

        while (min->left != nullptr) {
            min = min->left;
        }

        return min;
    }

    Node* max () {
        max(root);
    }

    Node* max (Node* max) {
        if (max == nullptr) {
            return nullptr;
        }

        while (max->right != nullptr) {
            max = max->right;
        }

        return max;
    }

    Node* search (const Comparable& data) const {
        return search(data, root);
    }

    void insert (const Comparable& data) {
        insert(data, root);
    }

    void remove (const Comparable& data) {
        remove(data, &root);
    }

    void print (std::ostream& out = std::cout) {
        print(root, out);
    }
private:
    Node* search (const Comparable& data, Node*const& t) const {
        if (t == nullptr || data == t->data) {
            return t;
        }

        if (data > t->data) {
            return search(data, t->right);
        } else {
            return search(data, t->left);
        }
    }

    void insert (const Comparable& data, Node*& t) {
        if (t == nullptr) {
            t = new Node(data);
        } else if (data < t->data) {
            insert(data, t->left);
        } else if (data > t->data) {
            insert(data, t->right);
        } else {
            ; // duplicate data - do nothing
        }
    }

    void remove (const Comparable& data, Node** current) {
        while (*current != nullptr && (*current)->data != data) {
            if (data < (*current)->data) {
                current = &((*current)->left);
            } else {
                current = &((*current)->right);
            }
        }

        if (*current == nullptr) {
            // data not in tree
            return;
        }

        // create a direct reference to the node pointer to overwrite it
        Node*& found = *current;

        if (found->left == nullptr && found->right == nullptr) {
            // no child nodes
            delete found;
            found = nullptr;
        } else if (found->left != nullptr && found->right != nullptr) {
            // two child nodes
            Node* minimum = min(found->right);
            found->data = minimum->data;
            remove(minimum->data, &(found->right));
        } else  {
            // one child node
            Node* next = (found->right != nullptr) ? found->right : found->left;
            delete found;
            found = next;
        }
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

    void preOrderTraversal (Node*& t, auto processNode) {
        if (t == nullptr) {
            return;
        }

        processNode(t);
        preOrderTraversal(t->left, processNode);
        preOrderTraversal(t->right, processNode);
    }

    void inOrderTraversal (Node*& t, auto processNode) {
        if (t == nullptr) {
            return;
        }

        inOrderTraversal(t->left, processNode);
        processNode(t);
        inOrderTraversal(t->right, processNode);
    }

    void postOrderTraversal (Node*& t, auto processNode) {
        if (t == nullptr) {
            return;
        }

        postOrderTraversal(t->left, processNode);
        postOrderTraversal(t->right, processNode);
        processNode(t);
    }
private:
    Node* root = nullptr;
};

#endif