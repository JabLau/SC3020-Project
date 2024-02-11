//
// Created by Ivan on 11/2/2024.
//

#include "Node.h"
#include <iostream>

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

bool Node::addKey(int key, int *address) {
    if (this->currKeyCount > 0) {
        bool added = false;
        // Has values, need to find spot to place it in
        // Check if any keys smaller than our new key
        for(int i=0; i < this->currKeyCount;i++) {
            if (this->keys[i] > key) {
                // Key "i" is bigger than new key
                // Have to move all keys backwards to fit new key
                for (int k=this->currKeyCount;k > i;k--) {
                    this->keys[k] = this->keys[k-1];
                    this->pointers[k] = this->pointers[k-1];
                }
                // Insert new Key and address
                this->keys[i] = key;
                this->pointers[i] = address;
                added = true;
                break;
             }
        }
        if (!added) {
            //All keys are smaller than current key, put in first free position
            this->keys[this->currKeyCount] = key;
            this->pointers[this->currKeyCount] = address;
        }
    }else {
        this->keys[0] = key;
        this->pointers[0] = address;
    }
    this->currKeyCount++;
    return true;
}

void Node::setParent(int *parentPointer) {
    this->parentPointer = parentPointer;
}

int *Node::getParent() {
    return this->parentPointer;
}

void Node::setNextNodePointer(int *nextLocation) {
    this->pointers[this->maxKeys] = nextLocation;
}

int *Node::getNextNodePointer() {
    return this->pointers[this->maxKeys];
}

void Node::printNode() {
    if (currKeyCount > 0) {
        for (int i=0; i < this->currKeyCount;i++) {
            cout << pointers[i] << "|" << keys[i] << "|";
        }
        cout << endl;
    }else {
        cout << "Node is empty" << endl;
    }

}