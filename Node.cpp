//
// Created by Ivan on 11/2/2024.
//

#include "Node.h"
#include "BPTree.h"
#include "Record.h"
#include <iostream>
#include <cmath>

using namespace std;

Node::Node(int maxKeys) {
    this->keys = new int[maxKeys]();
    this->pointers = new int*[maxKeys+1]();
    this->maxKeys = maxKeys;
}

Node::~Node() {

}

bool Node::isFull() {
    return (this->currKeyCount >= this->maxKeys);
}

bool Node::addFirstChild(int *address) {
    this->pointers[0] = address;
    return true;
}

bool Node::addChild(int key, int *address) {
    // Values already sorted
    this->keys[this->currKeyCount] = key;
    this->pointers[this->currKeyCount+1] = address;
    this->currKeyCount++;
    return true;

//  This was a sorting function but can remove if not needed anymore!
//    if (this->currKeyCount > 0) {
//        bool added = false;
//        // Has values, need to find spot to place it in
//        // Check if any keys smaller than our new key
//        for(int i=0; i < this->currKeyCount;i++) {
//            if (this->keys[i] > key) {
//                // Key "i" is bigger than new key
//                // Have to move all keys backwards to fit new key
//                for (int k=this->currKeyCount;k > i;k--) {
//                    this->keys[k] = this->keys[k-1];
//                    this->pointers[k] = this->pointers[k-1];
//                }
//                // Insert new Key and address
//                this->keys[i] = key;
//                this->pointers[i] = address;
//                added = true;
//                break;
//             }
//        }
//        if (!added) {
//            //All keys are smaller than current key, put in first free position
//            this->keys[this->currKeyCount] = key;
//            this->pointers[this->currKeyCount] = address;
//        }
//    }else {
//        this->keys[0] = key;
//        this->pointers[0] = address;
//    }
//    this->currKeyCount++;
//    return true;
}

bool Node::insertLeafNodeKey(int key, int *address) {
    // Value is already sorted, no need to check previous values
    this->keys[this->currKeyCount] = key;
    this->pointers[this->currKeyCount] = address;
    this->currKeyCount++;
    return true;
}

void Node::setLeafNode(bool isLeaf) {
    this->leafNode = isLeaf;
}

void Node::setNextNodePointer(int *nextLocation) {
    if (this->leafNode) {
        this->pointers[this->maxKeys] = nextLocation;
    }
}


int* Node::getNextNodePointer() {
    if (this->leafNode) {
        return this->pointers[this->maxKeys];
    }
    return nullptr;
}

void Node::printNode() {
    if (currKeyCount > 0) {
        for (int i=0; i < this->currKeyCount;i++) {
            cout << pointers[i] << "|" << keys[i] << "|";
        }
        if (!this->leafNode) {
            // For non-leaf nodes, print last address as well
            cout << pointers[this->currKeyCount];
        }
        cout << endl;
    }else {
        cout << "Node is empty" << endl;
    }
}

tempStruct Node::getKeyForTransfer() {
    tempStruct rtn = tempStruct();
    if (this->leafNode) {
        // Leaf Node
        // Return last key and second last pointer
        rtn.key = keys[this->currKeyCount-1];
        rtn.address = pointers[this->currKeyCount-1];
    } else {
        // Not Leaf Node
        // Return last key and  last pointer
        rtn.key = keys[this->currKeyCount-1];
        rtn.address = pointers[this->currKeyCount];
    }
    this->currKeyCount--;
    return rtn;
}

void Node::keyTransfer(int key, int* address) {
    if (this->leafNode) {
        // Check if has any keys
        if (this->currKeyCount > 0) {
            // Shift all keys & pointers backwards
            // TODO: Double Check if doing transfers will result in node becoming full?
            for (int i=this->currKeyCount; i > 0 ;i--) {
                this->keys[i] = this->keys[i-1];
                this->pointers[i] = this->pointers[i-1];
            }
        }
        this->keys[0] = key;
        this->pointers[0] = address;
    }else {
        // TODO: After verifying this code works 
        // Change this to > 0 and put for loop inside ltr
        // Then put duplicate code outside of if statement to run regardless of case
        if (this->currKeyCount > 0) {
            for (int i=this->currKeyCount; i > 0 ;i--) {
                this->keys[i] = this->keys[i-1];
                this->pointers[i+1] = this->pointers[i];
            }
        }// Retrieve Key to store as first key
        Node* tempNode = (Node*) this->pointers[0];
        Record* tempRecord = (Record*) tempNode->pointers[0];
        this->pointers[0] = address;
        this->keys[0] = tempRecord->numVotes;
        this->pointers[1] = (int*) tempNode;
    }
    this->currKeyCount++;
}

bool Node::nodeValid() {
    if (this->leafNode) {
        // Leaf Node check for floor((n+1)/2)
        return (this->currKeyCount >= floor((this->maxKeys+1)/2));
    }else {
        // Non-leaf node check for floor(n/2)
        return (this->currKeyCount >= floor(this->maxKeys/2));
    }
}