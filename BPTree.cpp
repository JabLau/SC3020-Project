//
// Created by Ivan on 11/2/2024.
//

#include "BPTree.h"
#include "Node.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
#include "Record.h"

using namespace std;

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

bool BPTree::bulkLoad(vector<tempStruct> &list, int size) {

    int level; // Track what level we are on curr
    Node* currNode; //Current node we working on
    Node *prevNode = nullptr; // Previous Node

    int n = 3;
    int totalNodes = 0;

    // Initalize First Level of nodes
    Node* first = new Node(n);
    this->rootNode = first; // Temp not actual root node!!
    currNode = first;
    currNode->setLeafNode(true);
    level++;
    totalNodes++;
    // cout << level << "|" << totalNodes << "|" << currNode << endl;
    for (int i=0;i < size;i++) {
        // Check if full
        if (currNode->isFull()) {
            // Curr node full, Create next node
            Node* nextNode = new Node(n);
            currNode->setNextNodePointer((int*)nextNode);
            prevNode = currNode;
            currNode = nextNode;
            currNode->setLeafNode(true);
            totalNodes++;
            // cout << level << "|" << totalNodes << "|" << nextNode << endl;
        }
        currNode->insertLeafNodeKey(list[i].key,list[i].address);
    }

    // Ensure last Leaf Node Valid, floor(n+1/2)
    ensureNodeValid(prevNode, currNode);

    // Only 1 node, which is root node
    if (totalNodes == 1) {
        return true;
    }

    int nodesInLevel;
    queue<Node*>* prevFifoQueue = nullptr; // For previous level
    queue<Node*>* currFifoQueue = new queue<Node*>(); // For current level
    Node *navNode;
    int queueSize;
    bool nodeFirstPtr = false; // Fresh node, need to ensure 1st item added is the pointer!
    do {
        // Next level in B Tree
        level++;
        currNode = nullptr;
        prevNode = nullptr;
        nodesInLevel = 0;
        // Get first node to navigate to
        if (level == 2) {
            navNode = this->rootNode;
        }else {
            navNode = prevFifoQueue->front();
            prevFifoQueue->pop();
        }
        do{
            if (currNode == nullptr || currNode->isFull()) {
                // Check if curr node full, need create next node
                Node* nextNode = new Node(n);
                nodeFirstPtr = true;
                totalNodes++;
                nodesInLevel++;
                // cout << level << "|" << totalNodes << "|" << nextNode << endl;
                currFifoQueue->push(nextNode); // Will include 1st node as well!

                if (nodesInLevel > 1) {
                    // If more than 1 node, means there is a previous node
                    prevNode = currNode;
                }
                currNode = nextNode;
            }

            if (nodeFirstPtr) {
                // Add first child, only pointer
                currNode->addFirstChild((int*)navNode);
                nodeFirstPtr = false;
            }else {
                // Add first key and pointer to node
                currNode->addChild(navNode->keys[0], (int*)navNode);
            }

            if (level == 2) {
                    // Currently indexing level 1, leaf node
                    navNode = (Node*) navNode->getNextNodePointer();
                }else {
                    // Indexing level 2 and above
                    if(!prevFifoQueue->empty()) {
                        navNode = prevFifoQueue->front();
                        prevFifoQueue->pop();
                    }else {
                        navNode = nullptr;
                    }
                }
        }while (navNode != nullptr); //navNode check for leaf node

        // Check if last node has floor(n/2) pointers
        ensureNodeValid(prevNode, currNode);

        // Copy currFifoQueue over
        if (prevFifoQueue != nullptr) {
            free(prevFifoQueue);
        }
        prevFifoQueue = currFifoQueue;
        currFifoQueue = new queue<Node*>();

    }while (prevFifoQueue->size() != 1);
    this->rootNode = currNode;

    cout << "Total Tree Levels:" << level << endl;
    cout << "Total Nodes in B+Tree:" << totalNodes << endl;

    return true;
}

void BPTree::ensureNodeValid(Node* prevNode, Node* currNode) {
    if (currNode->nodeValid() == false && prevNode != nullptr) {
        // Node invalid, borrow from prev node
        tempStruct transfer;
        do {
            // Transfer nodes from neighbour until valid
            transfer = prevNode->getKeyForTransfer();
            currNode->keyTransfer(transfer.key, transfer.address);
        }while (currNode->nodeValid() == false);
    }
}

void BPTree::printTree() {
    Node* navNode = this->rootNode;
    navNode->printNode();
}

void BPTree::findByRange(int start, int end) {
    // Implement checks to prevent end < start?

    Node *currNode = this->findNodeWithValue(start);
    if (currNode == nullptr) {
        cout << "Cannot find node with value" << endl;
        return;
    }

    double totalAvgRating = 0; 
    int recordCount = 0;

    bool overshot = false;
    Record* tempRecord;
    do {
        for (int i=0;i < currNode->currKeyCount;i++) {
            if (currNode->keys[i] >= start) {
                if (currNode->keys[i] > end) {
                    overshot = true;
                    break;
                }
                tempRecord = (Record*) currNode->pointers[i];
                totalAvgRating += tempRecord->avgRating;
                recordCount++;
            }
        }

        if (overshot) {
            break;
        }
        currNode = (Node*) currNode->getNextNodePointer();
    }while (currNode != nullptr);

    // Print Results
    cout << "Total Average of Average ratings:" << (totalAvgRating/recordCount) << endl;
    cout << "Total Records Accessed:" << recordCount << endl;
}

Node* BPTree::findNodeWithValue(int value) {
    Node* currNode = this->rootNode;
    bool foundNextNode;
    int indexNodesAccessed = 1; // Do We count Root node as Index Node Accessed?
    while (currNode->leafNode == false) {
        // Not leaf node
        foundNextNode = false;
        for (int i=0;i < currNode->currKeyCount;i++) {
            if (value <= currNode->keys[i]) {
                currNode = (Node*) currNode->pointers[i];
                foundNextNode = true;
                break;
            }
        }

        if (!foundNextNode) {
            // Did not find key smaller than value, use last pointer
            // TODO: Check if there is a pointer here? But should have right?
            currNode = (Node*) currNode->pointers[currNode->currKeyCount];
        }
        indexNodesAccessed++;
    }
    indexNodesAccessed--; // Last node accessed is a Leaf Node, Not counted!

    // Value < Smallest value in B+Tree, cannot find
    // Value > Largest value in B+Tree, cannot find
    if (value < currNode->keys[0] || value > currNode->keys[currNode->currKeyCount-1]) {
        return nullptr;
    }

    // Check if current leaf node has value
    // If not traverse nodes until find Node with Value
    bool foundValue = false;
    do {
        for (int i=0;i < currNode->currKeyCount;i++) {
            if (currNode->keys[i] == value) {
                foundValue = true;
                break;
            }
        }

        if (foundValue) {
            break; // Found, now GTFO of loop
        }
        currNode = (Node*) currNode->getNextNodePointer(); // Go to next node
    }while (currNode != nullptr);
    // Terminate if found the value or
    // If reach end of line without finding value
    // Either by traversing all the nodes till the end w/o finding the value or
    // This the only node and does not have the value

    if (foundValue) {
        cout << "Index nodes Accessed:" << indexNodesAccessed << endl;
        // Found value in this node, return it
        return currNode;
    }

    // Could not find value
    return nullptr;
}