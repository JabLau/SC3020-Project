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
    

    bool splitNode(Node* node, int key, int *address);
    bool insertKey(int key, int *address);
 //   bool bulkLoad(tempStruct *list, int size);
    bool bulkLoad(std::vector<tempStruct2> &list, int size);
    void ensureNodeValid(Node*, Node*);
    void printTree();
    void returnNodeCount();

    Node* findNodeWithValue(int value,bool);
    Node* findStartingNodeForRange(int value,int*);
    vector<vector<int*>>* findByRange(int, int,bool);
    vector<int*>* findByValue(int,bool);

    bool deleteNodes(int value);

    int getLowerBoundOfSubTree(Node*);

    void leafNodeCheck();
};

#endif //SC3020_PROJECT_BPTREE_H
