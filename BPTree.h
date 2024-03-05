//
// Created by Ivan on 11/2/2024.
//

#ifndef SC3020_PROJECT_BPTREE_H
#define SC3020_PROJECT_BPTREE_H

#include "Node.h"
#include "tempStruct.h"
#include <vector>


class BPTree {
public:
    int maxKeys; //Keys per node
    Node* rootNode = nullptr;

    BPTree(int maxKeys);

//    bool insertKey(int key, int* address);
 //   bool bulkLoad(tempStruct *list, int size);
    bool bulkLoad(std::vector<tempStruct2> &list, int size);
    void ensureNodeValid(Node*, Node*);
    void printTree();

    Node* findNodeWithValue(int value);
    Node* findStartingNodeForRange(int value);
    vector<vector<int*>> findByRange(int, int);
    vector<int*> findByValue(int);

    bool deleteNodes(int value);

    int getLowerBoundOfSubTree(Node*);
};

#endif //SC3020_PROJECT_BPTREE_H
