#ifndef BINARY_TREE_H
#define BINARY_TREE_H

template <typename Object>
struct BinaryTree {
    Object data;
    BinaryTree<Object>* left = nullptr;
    BinaryTree<Object>* right = nullptr;
};

#endif
