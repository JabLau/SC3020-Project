//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_BPTREE_H
#define SC3020_PROJECT_BPTREE_H

#include "Node.h"
#include "tempStruct.h"


class BPTree {
public:
    int maxKeys; //Keys per node
    Node* rootNode = nullptr;

    BPTree(int maxKeys);

//    bool insertKey(int key, int* address);
    bool bulkLoad(tempStruct *list, int size);

    void printTree();
};

#endif //SC3020_PROJECT_BPTREE_H
