//
// Created by Ivan on 11/2/2024.
//

#include "BPTree.h"
#include "Node.h"
#include <cmath>

BPTree::BPTree(int maxKeys) {
    this->maxKeys = maxKeys;
}

//bool BPTree::insertKey(int key, int *address) {
//    if (this->rootNode == nullptr) {
//        Node root = Node(this->maxKeys);
//        root.addChild(key, address);
//        root.isLeafNode(true);
//        this->rootNode = &root;
//    }else {
//        // Has nodes, find leaf node for key
//        Node currNode = *(this->rootNode);
//
//        // Search through tree to find leaf node
//        while (currNode.leafNode == false) {
//            int* nextNode = nullptr;
//            // Find part of current node that is bigger than key
//            for (int i=0;i<currNode.currKeyCount;i++) {
//                if (currNode.keys[i] > key) {
//                    nextNode = currNode.pointers[i];
//                }
//            }
//
//            if (nextNode == nullptr) {
//                if (currNode.isFull() == false) {
//                    nextNode = currNode.pointers[currNode.currKeyCount]; //Get pointer behind last key in Node
//                }
//                // Full but cannot find? deadend????
//                // Cause only pointer left is the one going to the next node in level
//            }
//
//            currNode = *nextNode;
//        }
//        // Found leaf node to put key in
//        // Check if leaf node full
//        if (currNode.isFull()) {
//            // Split node
//        }else {
//            currNode.addChild(key, address);
//        }
//
//    }
//    return false;
//}

bool BPTree::bulkLoad(tempStruct *list, int size) {

    Node* firstNode; //First node of level
    int level; // Track what level we are on curr
    Node* currNode; //Current node we working on
    Node *prevNode = nullptr; // Previous Node

    int n = 3;
    int level1_NodeCount = ceil(size/n);
    // Initalize first node
    Node first = Node(n);
    this->rootNode = &first; // Temp not actual root node!!
    firstNode = &first;
    currNode = &first;
    currNode->setLeafNode(true);
    int lv1_NodeCount = 1; // TODO: Remove for debug only!
    level++;
    // Level 1 of nodes
    for (int i=0;i < size;i++) {
        // Check if full
        if (currNode->isFull()) {
            // Create next node
            Node* nextNode = new Node(n);
            currNode->setNextNodePointer((int*)nextNode);
            prevNode = currNode;
            currNode = nextNode;
            currNode->setLeafNode(true);
            lv1_NodeCount++;
        }
        currNode->insertLeafNodeKey(list[i].key,list[i].address);
    }

    if (currNode->nodeValid() == false && prevNode != nullptr) {
        // Node invalid, borrow from prev
        tempStruct transfer;
        do {
            // Transfer nodes from neighbour until valid
            transfer = prevNode->getKeyForTransfer();
            currNode->keyTransfer(transfer.key, transfer.address);
        }while (currNode->nodeValid());
    }

    int nodesInLevel;
    Node* prevLevelRootNode = this->rootNode;
    Node *navNode;
    do {
        // Next level in B Tree
        level++;
        currNode = nullptr;
        prevNode = nullptr;
        nodesInLevel = 0;
        navNode = prevLevelRootNode;
        prevLevelRootNode = nullptr;
        do{
            if (currNode == nullptr || currNode->isFull()) {
                // Check if curr node full, need create next node
                Node* nextNode = new Node(n);
                if (currNode != nullptr) {
                    currNode->nextNodePointerTemp = (int*) nextNode;
                    currNode->setNextNode = true;
                    prevNode = currNode;
                }
                currNode = nextNode;
                nodesInLevel++;

                currNode->addFirstChild((int*)navNode);
                navNode->setParent((int*)currNode);
                navNode = (Node*) navNode->getNextNodePointer();

                if (prevLevelRootNode == nullptr) {
                    prevLevelRootNode = currNode;
                }
            }

            if (navNode == nullptr) {
                // No keys in current node = need to borrow
            }else {
                // Insert first value and pointer to node
                currNode->addChild(navNode->keys[0], (int*)navNode);
                navNode->setParent((int*)currNode);
                navNode = (Node*) navNode->getNextNodePointer();
            }
        }while (navNode != nullptr);
        // Check if last node has floor(n/2) pointers
        if (currNode->nodeValid() == false && prevNode != nullptr) {
            // Node invalid, borrow from prev
            tempStruct transfer;
            do {
                // Transfer nodes from neighbour until valid
                transfer = prevNode->getKeyForTransfer();
                currNode->keyTransfer(transfer.key, transfer.address);
            }while (currNode->nodeValid());
        }
        // For last node of current level
    }while (nodesInLevel != 1);
    this->rootNode = currNode;

    // TODO: Plan how many Level 1 nodes we need to have for the tree
    // Do a loop for each level, create a array for first node in level & node count in level
    // 1 while loop outside to check if only 1 node in curr level
    // 1 while loop inside to create nodes for level

    return false;
}

void BPTree::printTree() {
    Node* navNode = this->rootNode;
    // while (navNode != nullptr) {
        navNode->printNode();
        // navNode = (Node*)navNode->getNextNodePointer();
    // }
}