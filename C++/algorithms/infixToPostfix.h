#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib> /* strtol */
#include "../data-structures/Stack.h"
#include "../utils.h"

std::vector<std::string> infixToPostfix(char* infix) {
    std::vector<std::string> result;
    Stack<char> operatorStack;
    long int currInt;
    // keep track of open parens
    int parens = 0;
    std::unordered_map<char, int> rank;

    // treat '(' as low-rank so other operators won't pop it off the stack
    rank['('] = -1;
    rank['+'] = 0;
    rank['-'] = 0;
    rank['*'] = 1;
    rank['/'] = 1;

    for (char* currPtr = infix; *currPtr; infix = currPtr) {
        currInt = strtol(currPtr, &currPtr, 10);

        if (infix != currPtr) {
            // we found a number
            result.push_back(std::to_string(currInt));
            continue;
        }

        while (*currPtr == ' ') ++currPtr;
        if (*currPtr == 0) break;

        // we found an operator

        // handle parens first since they don't participate in rankings
        if (*currPtr == ')') {
            if (parens == 0) {
                throw CustomException("Unbalanced parens!");
            }

            while (operatorStack.top() != '(') {
                // pop all operators inside parens
                result.push_back(std::string(1, operatorStack.top()));
                operatorStack.pop();
            }
            // pop off the (
            operatorStack.pop();
            --parens;
        } else {
            if (*currPtr == '(') {
                ++parens;
            } else {
                // pop off higher-ranked operators before dealing with lower-ranking operator
                while (!operatorStack.empty() && rank[operatorStack.top()] >= rank[*currPtr]) {
                    result.push_back(std::string(1, operatorStack.top()));
                    operatorStack.pop();
                }
            }

            operatorStack.push(*currPtr);
        }

        // move past the operator
        ++currPtr;
    }

    if (parens != 0) {
        throw CustomException("Unbalanced parens!");
    }

    while (!operatorStack.empty()) {
        result.push_back(std::string(1, operatorStack.top()));
        operatorStack.pop();
    }

    return result;
}

#endif
