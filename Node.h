//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_NODE_H
#define SC3020_PROJECT_NODE_H


class Node {
public:
    bool leafNode;
    int *keys; // Number of votes
    int* *pointers;
    int maxKeys;
    int currKeyCount = 0;
    int* parentPointer;

    Node(int maxKeys);
    ~Node();
    bool isFull();
    bool addKey(int key, int* address);
    bool insertKeySorted(int key, int* address);
    void printNode();

    // Set if leaf node
    void isLeafNode(bool isLeaf);

    // Parent Node
    void setParent(int* parentPointer);
    int* getParent();
    // Pointer to next node in same level, aka pointer n+1
    void setNextNodePointer(int* nextLocation);
    int* getNextNodePointer();

    bool clearKeys();
    void setPtr(int index, int* address);
};


#endif //SC3020_PROJECT_NODE_H
