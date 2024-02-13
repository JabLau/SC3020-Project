//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_NODE_H
#define SC3020_PROJECT_NODE_H


class Node {
public:
    bool leafNode = false;
    int *keys; // Number of votes
    int* *pointers;
    int maxKeys;
    int currKeyCount = 0;
    int* parentPointer;
    bool setNextNode = false;
    int* nextNodePointerTemp; // Temp until i figure out how to not use this

    Node(int maxKeys);
    ~Node();
    bool isFull();
    bool addFirstChild(int *address); // 1st child has no key value
    bool addChild(int key, int* address); // 2nd onwards has key value
    bool insertLeafNodeKey(int key, int* address); //For leaf node
    void printNode();

    // Set if leaf node
    void setLeafNode(bool isLeaf);

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
