//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_NODE_H
#define SC3020_PROJECT_NODE_H

#include "tempStruct.h"

class Node {
public:
    bool leafNode = false;
    int *keys; // Number of votes
    int* *pointers; // I want to change to Node* but we also store disk address how ah?
    int maxKeys;
    int currKeyCount = 0;
    Node* parentPtr; // Might be unsafe, cause comparing nullptr is inconsistent!
    bool parentSet = false;

    Node(int maxKeys);
    ~Node();
    bool isFull();
    bool addFirstChild(int *address); // 1st child has no key value
    bool addChild(int key, int* address); // 2nd onwards has key value
    bool insertLeafNodeKey(int key, int* address); //For leaf node
    void printNode();

    // Set if leaf node
    void setLeafNode(bool isLeaf);

    // To get next node pointer for leaf nodes
    void setNextNodePointer(int*);
    int* getNextNodePointer();
    
    void setParentPointer(Node*);
    Node* getParentPointer();
    void removeParentPointer();
    
    tempStruct getKeyForTransfer();
    void keyTransfer(int key, int* address);

    bool nodeValid();

    void deleteKey(int);

    // For Placing Borrowed Key
    void addChildFront(int key, int* address); // 2nd onwards has key value

    // For Borrowing from Sibling Nodes
    bool borrowKeyCheck();
    tempStruct borrowKeyCauseDeletion(bool);
    tempStruct getFirstPointerForBorrow();

    int getLowerBoundKey(Node*);
    void mergeNode(Node*);

    void removeChildNode(int*);
};


#endif //SC3020_PROJECT_NODE_H
