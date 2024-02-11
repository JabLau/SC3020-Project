//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_NODE_H
#define SC3020_PROJECT_NODE_H


class Node {
private:
    int *keys; // Number of votes
    int* *pointers;
    int maxKeys;
    int currKeyCount = 0;
    int* parentPointer;

public:
    Node(int maxKeys);
    ~Node();
    bool isFull();
    bool addKey(int key, int* address);
    void printNode();

    // Parent Node
    void setParent(int* parentPointer);
    int* getParent();
    // Pointer to next node in same level, aka pointer n+1
    void setNextNodePointer(int* nextLocation);
    int* getNextNodePointer();
};


#endif //SC3020_PROJECT_NODE_H
