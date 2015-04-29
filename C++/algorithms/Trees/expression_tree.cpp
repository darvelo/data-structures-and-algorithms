#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> /* strtol */
#include <functional>
#include "../infixToPostfix.h"
#include "../../data-structures/BinaryTree.h"
#include "../../data-structures/Stack.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::function;

void printNode(BinaryTree<string>* node) {
    cout << node->data;
}

void printInfix(BinaryTree<string>* tree) {
    if (!tree) return;
    cout << " (";
    printInfix(tree->left);
    printNode(tree);
    printInfix(tree->right);
    cout << ") ";
}

long int getAnswer(BinaryTree<string>* tree) {
    if (!tree->left || !tree->right) return strtol(tree->data.c_str(), nullptr, 10);
    if (tree->data == "+") return getAnswer(tree->left) + getAnswer(tree->right);
    if (tree->data == "-") return getAnswer(tree->left) - getAnswer(tree->right);
    if (tree->data == "*") return getAnswer(tree->left) * getAnswer(tree->right);
    if (tree->data == "/") return getAnswer(tree->left) / getAnswer(tree->right);
    return 0;
}

BinaryTree<string>* postfixToExpressionTree(vector<string>& postfix) {
    BinaryTree<string>* result = nullptr;
    Stack<BinaryTree<string>*> stack;

    for (auto& str : postfix) {
        BinaryTree<string>* node = new BinaryTree<string>;
        node->data = str;

        if (str == "+" ||
            str == "-" ||
            str == "*" ||
            str == "/")
        {
            BinaryTree<string>* right = stack.top();
            stack.pop();
            BinaryTree<string>* left = stack.top();
            stack.pop();

            node->left = left;
            node->right = right;
        }

        stack.push(node);
    }

    result = stack.top();
    stack.pop();

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " \"[numeric expression]\"" << endl;
        cout << "  e.g. " << argv[0] << " \"1 + (10 * 8) / 2 - 6\"" << endl;
        cout << "  e.g. " << argv[0] << " \"1 * 3 + 8 * (10 * 8) / 2 - 6\"" << endl;
        return 1;
    }

    cout << "Note: The floor of the result is used when performing integer division." << endl << endl;

    vector<string> postfix = infixToPostfix(argv[1]);
    BinaryTree<string>* expressionTree = postfixToExpressionTree(postfix);

    cout << "Postfix conversion:" << endl;
    for (auto& s : postfix) {
        cout << s << " ";
    }
    cout << endl << endl;

    cout << "Infix reconstitution:" << endl;
    printInfix(expressionTree);
    cout << endl << endl;

    cout << "Math result:" << endl;
    cout << getAnswer(expressionTree) << endl;
    cout << endl;

    return 0;
}
