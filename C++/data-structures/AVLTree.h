#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <functional> /* less, function */
#include <utility> /* move, forward */
#include <algorithm> /* max */

template <typename Object, typename Comparator=std::less<Object>>
class AVLTree {
private:
    struct Node {
        Object data;
        Node* left = nullptr;
        Node* right = nullptr;
        int height;

        Node (const Object& x, Node* _left = nullptr, Node* _right = nullptr, int _height = 0)
            : data(x), left(_left), right(_right), height(_height) { }
        Node (Object&& x, Node* _left = nullptr, Node* _right = nullptr, int _height = 0)
            : data(std::move(x)), left(_left), right(_right), height(_height) { }
    };
public:
    AVLTree() { }
    AVLTree(const AVLTree& rhs) : root{nullptr} {
        *this = rhs;
    }
    AVLTree(AVLTree&& rhs) : root{nullptr} {
        *this = std::forward<AVLTree>(rhs);
    }
    AVLTree& operator=(const AVLTree& rhs) {
        root = clone(rhs.root);
    }
    AVLTree& operator=(AVLTree&& rhs) {
        clear();
        root = std::move(rhs.root);
        rhs.root = nullptr;
    }

    ~AVLTree() {
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

    int height(Node* t) const {
        return t == nullptr ? -1 : t->height;
    }

    void rotateWithLeftChild(Node*& t) {
        Node* k = t->left;
        t->left = k->right;
        k->right = t;
        t->height = 1 + std::max(height(t->left), height(t->right));
        k->height = 1 + std::max(height(k->left), t->height);
        t = k;
    }

    void rotateWithRightChild(Node*& t) {
        Node* k = t->right;
        t->right = k->left;
        k->left = t;
        t->height = 1 + std::max(height(t->left), height(t->right));
        k->height = 1 + std::max(height(k->right), t->height);
        t = k;
    }

    void doubleRotateWithLeftChild(Node*& t) {
        rotateWithRightChild(t->left);
        rotateWithLeftChild(t);
    }

    void doubleRotateWithRightChild(Node*& t) {
        rotateWithLeftChild(t->right);
        rotateWithRightChild(t);
    }

    void balance(Node*& t) {
        if (t == nullptr) {
            return;
        }

        // if left is higher than right
        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
            // if the height runs higher on the outer left margins of the subtree.
            // the `>=` allows deletions to rotate correctly when the subtree
            // on the opposite subtree of the deletion has subtrees of equal height
            if (height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);
            // if the height runs higher on the inner subtree of the left node
            } else {
                doubleRotateWithLeftChild(t);
            }
        // if right is higher than left
        } else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
            // if the height runs higher on the outer right margins of the subtree.
            // the `>=` allows deletions to rotate correctly when the subtree
            // on the opposite subtree of the deletion has subtrees of equal height
            if (height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);
            // if the height runs higher on the inner subtree of the right node
            } else {
                doubleRotateWithRightChild(t);
            }
        }

        t->height = 1 + std::max(height(t->left), height(t->right));
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

        balance(t);
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

        balance(t);
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

        balance(t);
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
            out << "data: " << t->data << ", height = " << t->height;

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

    static const int ALLOWED_IMBALANCE = 1;
    Comparator isLessThan;
    Node* root = nullptr;
};

#endif
