#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iostream>
#include <functional> /* less, function */
#include <utility> /* move, forward */
#include <algorithm> /* max */

template <typename Object, typename Comparator=std::less<Object>>
class RedBlackTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        Object data;
        Node* left = nullptr;
        Node* right = nullptr;
        Color color = BLACK;

        Node (const Object& x = Object{}, Node* _left = nullptr, Node* _right = nullptr, Color _color = BLACK)
            : data(x), left(_left), right(_right), color(_color) { }
        Node (Object&& x, Node* _left = nullptr, Node* _right = nullptr, Color _color = BLACK)
            : data(std::move(x)), left(_left), right(_right), color(_color) { }
    };
public:
    explicit RedBlackTree(const Object& negInfinity) {
        nullNode = new Node;
        nullNode->left = nullNode->right = nullNode;

        header = new Node(negInfinity);
        header->left = header->right = nullNode;
    }
    RedBlackTree(const RedBlackTree& rhs) {
        nullNode = new Node;
        nullNode->left = nullNode->right = nullNode;

        header = new Node(rhs.header->data);
        header->left = nullNode;
        header->right = clone(rhs.header->right);
    }
    RedBlackTree(RedBlackTree&& rhs) {
        header = rhs.header;
        nullNode = rhs.nullNode;
        rhs.header = nullptr;
        rhs.nullNode = nullptr;
    }
    ~RedBlackTree() {
        clear(header);
        delete nullNode;
    }

    Node* min () const {
        min(header->right);
    }

    Node* min (Node* min) const {
        if (min == nullNode) {
            return nullptr;
        }

        while (min->left != nullNode) {
            min = min->left;
        }

        return min;
    }

    Node* max () const {
        max(header->right);
    }

    Node* max (Node* max) const {
        if (max == nullNode) {
            return nullptr;
        }

        while (max->right != nullNode) {
            max = max->right;
        }

        return max;
    }

    Node* search (const Object& data) const {
        return search(data, header->right);
    }

    bool contains(const Object& data) const {
        return search(data, header->right) != nullNode;
    }

    void insert (const Object& data) {
        current = parent = grandparent = header;
        nullNode->data = data;

        while (current->data != data) {
            greatGrandparent = grandparent; grandparent = parent; parent = current;
            current = isLessThan(data, current->data) ? current->left : current->right;

            // check if two red children; fix if so
            if (current->left->color == RED && current->right->color == RED) {
                handleReorient(data);
            }
        }

        // insertion fails if data is already present
        if (current != nullNode) {
            return;
        }

        current = new Node(data, nullNode, nullNode);

        // attach to parent
        if (isLessThan(data, parent->data)) {
            parent->left = current;
        } else {
            parent->right = current;
        }

        handleReorient(data);
    }

    void remove (const Object& data) {
    }

    void clear () {
        clear(header->right);
        header->right = nullNode;
    }

    void print (std::ostream& out = std::cout) {
        if (header->right == nullNode) {
            out << "Empty tree" << std::endl;
        } else {
            print(header->right, out);
        }
    }
private:
    Node* search (const Object& data, Node*const& t) const {
        if (t == nullNode) {
            return nullptr;
        }

        if (isLessThan(t->data, data)) {
            return search(data, t->right);
        } else if (isLessThan(data, t->data)){
            return search(data, t->left);
        } else {
            return t;
        }
    }

    void rotateWithLeftChild(Node*& t) {
        Node* k = t->left;
        t->left = k->right;
        k->right = t;
        t = k;
    }

    void rotateWithRightChild(Node*& t) {
        Node* k = t->right;
        t->right = k->left;
        k->left = t;
        t = k;
    }

    /**
     * Internal routine that performs a single or double rotation.
     * Because the result is attached to the parent, there are four cases.
     * Called by handleReorient.
     * `item` is the item in handleReorient.
     * `theParent` is the parent of the root of the rotated subtree.
     * Return the root of the rotated subtree.
     */
    Node* rotate(const Object& item, Node* theParent) {
        if (isLessThan(item, theParent->data)) {
            if (isLessThan(item, theParent->left->data)) {
                rotateWithLeftChild(theParent->left); // LL
            } else {
                rotateWithRightChild(theParent->left); // LR
            }
            return theParent->left;
        } else {
            if (isLessThan(item, theParent->right->data)) {
                rotateWithLeftChild(theParent->right); // RL
            } else {
                rotateWithRightChild(theParent->right); // RR
            }
            return theParent->right;
        }
    }

    /**
     * Internal routine that is called during an insertion if a node has two red children.
     * Performs flips and rotations. `item` is the item being inserted.
     */
    void handleReorient(const Object& item) {
        // flip color
        current->color = RED;
        current->left->color = BLACK;
        current->right->color = BLACK;

        if (parent->color == RED) {
            // have to rotate
            grandparent->color = RED;
            if (isLessThan(item, grandparent->data) != isLessThan(item, parent->data)) {
                // start double rotation
                parent = rotate(item, grandparent);
            }
            current = rotate(item, greatGrandparent);
            current->color = BLACK;
        }

        header->right->color = BLACK;
    }

    void clear (Node*& t) {
        if (t != nullNode) {
            clear(t->left);
            clear(t->right);
            delete t;
            t = nullptr;
        }
    }

    Node* clone (Node* t) const {
        // cannot test against nullNode,
        // because copy constructor will reference this->nullNode,
        // NOT rhs.nullNode.
        if (t == t->left) {
            return nullNode;
        }
        return new Node(t->data, clone(t->left), clone(t->right), t->color);
    }

    void print (Node* t, std::ostream& out = std::cout) {
        Node* nullNode = this->nullNode;

        preOrderTraversal(t, [&out, nullNode] (Node* t) {
            out << "data: " << t->data << ", color = " << (t->color == BLACK ? "BLACK" : "RED");

            out << ", left: ";
            (t->left  != nullNode) ? out << t->left->data  : out << "null";

            out << ", right: ";
            (t->right != nullNode) ? out << t->right->data : out << "null";

            out << std::endl;
        });
        out << std::endl;
    }

    void preOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullNode) {
            return;
        }

        processNode(t);
        preOrderTraversal(t->left, processNode);
        preOrderTraversal(t->right, processNode);
    }

    void inOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullNode) {
            return;
        }

        inOrderTraversal(t->left, processNode);
        processNode(t);
        inOrderTraversal(t->right, processNode);
    }

    void postOrderTraversal (Node*& t, std::function<void(Node*)> processNode) {
        if (t == nullNode) {
            return;
        }

        postOrderTraversal(t->left, processNode);
        postOrderTraversal(t->right, processNode);
        processNode(t);
    }

    Comparator isLessThan;
    // the tree header (MUST contain a value less than or equal to all others)
    Node* header;
    Node* nullNode;
    // used in insert routine and its helpers (logically static)
    Node* current;
    Node* parent;
    Node* grandparent;
    Node* greatGrandparent;
};

#endif
