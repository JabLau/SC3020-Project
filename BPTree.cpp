//
// Created by Ivan on 11/2/2024.
//

#include "BPTree.h"
#include "Node.h"

BPTree::BPTree(int maxKeys) {
    this->maxKeys = maxKeys;
}

bool BPTree::insertKey(int key, int *address) {
    if (this->rootNode == nullptr) {
        Node root = Node(this->maxKeys);
        root.addKey(key, address);
        root.isLeafNode(true);
        this->rootNode = &root;
    }else {
        // Has nodes, find leaf node for key
        Node currNode = *(this->rootNode);

        // Search through tree to find leaf node
        while (currNode.leafNode == false) {
            int* nextNode = nullptr;
            // Find part of current node that is bigger than key
            for (int i=0;i<currNode.currKeyCount;i++) {
                if (currNode.keys[i] > key) {
                    nextNode = currNode.pointers[i];
                }
            }

            if (nextNode == nullptr) {
                if (currNode.isFull() == false) {
                    nextNode = currNode.pointers[currNode.currKeyCount]; //Get pointer behind last key in Node
                }
                // Full but cannot find? deadend????
                // Cause only pointer left is the one going to the next node in level
            }

            currNode = *nextNode;
        }
        // Found leaf node to put key in
        // Check if leaf node full
        if (currNode.isFull()) {
            // Split node
        }else {
            currNode.addKey(key, address);
        }

    }
    return false;
}