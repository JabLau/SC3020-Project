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


Node* Node::splitNode(int key, int* address){
    /*This function aims to split the 2 nodes when full. It accepts key to be inserted and the address of the
     * node to be split.
     * With the current, we need to compare it with the middle key. Middle Key = n/2+1
     * For both leaf node and internal node:
         * if key< middle key, transfer from the middle key onwards
         * if key> middle key, transfer from after the middle key onwards.
     *
     * 2 Questions:
     * How to delete key
     * Does add child deal with lack of pointers?
     */
    Node *newNode = new Node(this->maxKeys);
    if (this->leafNode) {
        //Perform splitting here.
        newNode->setLeafNode(true);
        newNode->setParentPointer(this->getParentPointer());
        int middleIndex = (this->maxKeys) / 2 ;
        if(key>this->keys[middleIndex])
        {
            //Since key>middle key, Transfer from after the middle key onwards
            for(int i=middleIndex+1; i<this->maxKeys; i++)
            {
                newNode->addChild(this->keys[i],this->pointers[i]);
                this->currKeyCount--;
            }
            newNode->addChild(key,address);
        }else {
            //Since key< middle key, transfer from the middle key onwards
            for (int i = middleIndex; i < this->maxKeys; i++) {
                newNode->addChild(this->keys[i], this->pointers[i]);
                this->currKeyCount--;
            }
            this->addChild(key, address);
        }
        newNode->setNextNodePointer(this->getNextNodePointer()); //worse case is set nullptr
        this->setNextNodePointer((int*)newNode);

    }else {
        newNode->setParentPointer(this->getParentPointer());
        // Internal Node
        int middleIndex = (this->maxKeys+(this->maxKeys%2)) / 2;

        if(key>this->keys[middleIndex-1])
        {
            // Belong to right side
            //Since key>middle key, Transfer from after the middle key onwards
            for(int i=middleIndex+1; i<=this->maxKeys; i++)
            {
                newNode->addChild(this->keys[i-1],this->pointers[i]);
                this->currKeyCount--;
            }
            newNode->addChild(key,address);
        }else {
            //Since key< middle key, transfer from the middle key onwards
            for (int i = middleIndex; i <=this->maxKeys; i++) {
                newNode->addChild(this->keys[i-1], this->pointers[i]);
                this->currKeyCount--;
            }
            this->addChild(key, address);
        }
        this->setChildNodeParent();
        newNode->setChildNodeParent();







//        if (key ) {
//            // Belongs to Right Node, Transfer 1 less pointer over
//            middleIndex++;
//        }
//
//        int j = 0;
//        for (int i = middleIndex; i < currKeyCount; i++, j++) {
//            newNode->addChild(this->keys[i],this->pointers[i]);
////            newNode->keys[j] = this->keys[i];
////            newNode->pointers[j] = this->pointers[i];
////            newNode->currKeyCount++;
//            j++;
//        }
//
//        newNode->pointers[j] = this->pointers[this->currKeyCount];
//
//        // Update current key count
//        this->currKeyCount = middleIndex;
//        if (this->keys[middleIndex - 1] < key){
//            // New key belongs to new node
//            newNode->addChild(key, address);
//        } else {
//            // New key belongs to curr node
//            this->addChild(key, address);
//        }
//
//
//        // Find Middle
//        // Transfers keys over to newNode
//        // Update curr key count in currNode
//        // Check if new key is smaller than newNode's 1st key
//        // If bigger, newNode->addChild
//        // Is Smaller, this->addChild
//
//        // Return splitNode
//        // Accept in node pointer
//        // Transfer keys and pointers from current node to the new node
//
//
//        // Update current node's key count
//        this->currKeyCount = middleIndex - 1;
//
//        // Set leaf status
//        newNode->leafNode = true;
    }
    return newNode;
}

void Node::setChildNodeParent() {
    if (!this->leafNode) {
        for (int i = 0; i <= this->currKeyCount; i++) {
            Node *temp = (Node*) this->pointers[i];
            temp->setParentPointer(this);
        }
    }
}

void Node::updateParentNode() {
    Node* parentPtr = this->getParentPointer();
    if (parentPtr != nullptr) {
        parentPtr->parentUpdateChildKey((int*)this);
    }
}

void Node::parentUpdateChildKey(int* updatedChild) {
    for (int i=0; i <= currKeyCount;i++) {
        if (pointers[i] == updatedChild) {
            if (i>0) {
                Node* child = (Node*)pointers[i];
                this->keys[i-1] = child->getSelfLowerBoundKey();
                this->updateParentNode();
            }
            break;
        }
    }
}

bool Node::addChild(int key, int *address) {
    // Values already sorted
//    this->keys[this->currKeyCount] = key;
//    this->pointers[this->currKeyCount+1] = address;
//    this->currKeyCount++;
//    return true;

    //This function accepts parents node address and child key value to add a child into the B+ Tree.
    /*This function checks if it is a leaf node. If it is a leaf node, then slot into the node accordingly based on the
     * key value against the search value.
     */
    if (this->leafNode) {
        if (this->currKeyCount > 0) {
            if (key >= this->keys[this->currKeyCount - 1]) {
                //All keys are smaller than current key, put in first free position
                this->keys[this->currKeyCount] = key;
                this->pointers[this->currKeyCount] = address;
            } else {
                // Has values, need to find spot to place it in
                // Check if any keys smaller than our new key
                for (int i = 0; i < this->currKeyCount; i++) {
                    if (this->keys[i] > key) {
                        // Key "i" is bigger than new key
                        // Have to move all keys backwards to fit new key
                        for (int k = this->currKeyCount; k > i; k--) {
                            this->keys[k] = this->keys[k - 1];
                            this->pointers[k] = this->pointers[k - 1];
                        }
                        // Insert new Key and address
                        this->keys[i] = key;
                        this->pointers[i] = address;
                        if (i==0) {
                            this->updateParentNode();
                        }
                        break;
                    }
                }
            }
        } else {
            this->keys[0] = key;
            this->pointers[0] = address;
            this->updateParentNode();
        }
        this->currKeyCount++;
        return true;
    }else {
        Node* child = (Node*) address;
        child->setParentPointer(this);
        // Non Leaf Node
        if (this->firstPtr == false){
            // First Child of Internal Node
            this->pointers[0] = address;
            this->firstPtr = true;
        }else {
            // If you're here
            // Case 1: First pointer added, no keys added yet
            if (this->currKeyCount <= 0) {
                Node *firstPtr = (Node *) this->pointers[0];
                int firstPtrLB = firstPtr->getSelfLowerBoundKey();
                if (key < firstPtrLB) {
                    // Case 1.1: New key smaller than first ptr
                    this->pointers[1] = this->pointers[0];
                    this->keys[0] = firstPtrLB;
                    this->pointers[0] = address;
                    this->currKeyCount++;
                }else {
                    // Case 1.2: New key bigger than first ptr
                    this->keys[0] = key;
                    this->pointers[1] = address;
                    this->currKeyCount++;
                }
            }else {
            // Case 2: Keys added, find position to insert new key
                for (int i = 0; i < this->currKeyCount; i++) {
                    if (key < this->keys[i]) {
                        // Case 2.1: New key added is smaller than smallest key
                        if (i==0) {
                            Node *firstPtr = (Node *) this->pointers[0];
                            int firstPtrLB = firstPtr->getSelfLowerBoundKey();
                            // Is key smaller than first ptr
                            if (key >= firstPtrLB) {
                                // Key Bigger
                                // 0 1 2
                                //0 1 2 3
                                // 4 3 2 (stop at 2, cause 2-2=key 0 alr)
                                for (int k=this->currKeyCount+1;k>1;k--) {
                                    this->keys[k-1] = this->keys[k-2];
                                    this->pointers[k] = this->pointers[k-1];
                                }
                                this->keys[0] = key;
                                this->pointers[1] = address;
                            }else {
                                // Key Smaller than first pointer's LB
                                for (int k=this->currKeyCount+1;k>0;k++) {
                                    if (k>1) {
                                        this->keys[k-1] = this->keys[k-2];
                                    }else {
                                        // K = 1
                                        this->keys[0] = firstPtrLB;
                                    }
                                    this->pointers[k] = this->pointers[k-1];
                                }
                                this->pointers[0] = address;
                                this->updateParentNode();
                            }
                        }else {
                            // Case 2.2: New key added is in the middle of keys
                            // 1 2 3
                            //0 1 2 3
                            // 1 4 2 3
                            //0 1 4 2 3
                            // 3 + 1 = 4
                            // 4 3
                            for (int k=this->currKeyCount+1;k>(i+1);k--) {
                                this->keys[k-1] = this->keys[k-2];
                                this->pointers[k] = this->pointers[k-1];
                            }
                            this->keys[i] = key;
                            this->pointers[i+1] = address;
                        }
                        this->currKeyCount++;
                        break;
                    }else if(i == this->currKeyCount-1){
                        // Case 2.3: New Key is larger than all keys
                        this->keys[this->currKeyCount] = key;
                        this->pointers[this->currKeyCount+1] = address;
                        this->currKeyCount++;
                        break;
                    }
                }
            }
        }
        return true;
    }
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
    if (nextLocation == nullptr) {
        return;
    }

    if (this->leafNode) {
        this->pointers[this->maxKeys] = nextLocation;
        this->nextNodeSet = true;
    }
}


int* Node::getNextNodePointer() {
    if (this->leafNode && this->nextNodeSet) {
        return this->pointers[this->maxKeys];
    }
    return nullptr;
}

void Node::setParentPointer(Node* ptr) {
    this->parentPtr = ptr;
    this->parentSet = true;
}

Node* Node::getParentPointer() {
    if (this->parentSet == true) {
        return this->parentPtr;
    }else {
        return nullptr;
    }
}

void Node::removeParentPointer() {
    this->parentSet = false;
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

// Receive Key From Left Sibling
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

        // Get Lower Bound of Subtree
        Node* tempNode = (Node*) this->pointers[0];
        this->pointers[0] = address;
        this->keys[0] = getLowerBoundKey(tempNode);
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

bool Node::canAddChild() {
    return (this->currKeyCount + 1 <= this->maxKeys);
}


// Leaf node only
bool Node::borrowKeyCheck() {
    // If Borrowing equivalent to -1 key count
    if (this->leafNode) {
        // Leaf Node check for floor((n+1)/2)
        return (this->currKeyCount-1 >= floor((this->maxKeys+1)/2));
    }else {
        // Non-leaf node check for floor(n/2)
        return (this->currKeyCount-1 >= floor(this->maxKeys/2));
    }
} 
// When deletion and need to borrow key, call this from sibling node
tempStruct Node::borrowKeyCauseDeletion(bool leftSibling) {
    tempStruct t;
    if (leftSibling) {
        // Borrow Last Node
        return getKeyForTransfer();
    }else {
        // Borrow First Node
        return getFirstPointerForBorrow();
    }
}

tempStruct Node::getFirstPointerForBorrow() {
    tempStruct rtn = tempStruct();
    if (this->leafNode) {
        // Leaf Node
        // Return last key and second last pointer
        rtn.key = keys[0];
        rtn.address = pointers[0];
        for (int i=0;i < (this->currKeyCount-1);i++) {
            this->keys[0] = this->keys[i+1];
            this->pointers[0] = this->pointers[i+1];
        }
    } else {
        // Not Leaf Node
        // Return last key and  last pointer
        rtn.key = 0;
        rtn.address = pointers[0];

        this->pointers[0] = this->pointers[1];
        for (int i=0;i < (this->currKeyCount-1);i++) {
            this->keys[0] = this->keys[i+1];
            this->pointers[i+1] = this->pointers[i+2];
        }
    }
    this->currKeyCount--;
    return rtn;
}

void Node::mergeNode(Node* mergeNode) {
    // For Merging right node to left node only!
    if (this->leafNode) {
        for (int i = 0; i < mergeNode->currKeyCount;i++) {
            this->keys[this->currKeyCount] = mergeNode->keys[i];
            this->pointers[this->currKeyCount] = mergeNode->pointers[i];
            this->currKeyCount++;
        }
    }else {
        for (int i = 0; i <= mergeNode->currKeyCount;i++) {
            this->keys[this->currKeyCount] = getLowerBoundKey((Node*) mergeNode->pointers[i]);
            Node* childNode = (Node*)mergeNode->pointers[i];
            childNode->setParentPointer(this);
            this->pointers[this->currKeyCount+1] = mergeNode->pointers[i];
            this->currKeyCount++;
        }
    }
}

int Node::getLowerBoundKey(Node* curr) {
    while (curr->leafNode == false) {
        curr =(Node*) curr->pointers[0];
    }
    return curr->keys[0];
}

int Node::getSelfLowerBoundKey() {
    Node* curr = this;
    while (curr->leafNode == false) {
        curr =(Node*) curr->pointers[0];
    }
    return curr->keys[0];
}

// For leaf node only
void Node::addChildFront(int key, int* address) {
    if (this->leafNode) {
        for (int i=0;i < this->currKeyCount;i++) {
            this->keys[i+1] = this->keys[i];
            this->pointers[i+1] = this->pointers[i];
        }
        this->keys[0] = key;
        this->pointers[0] = address;
        this->currKeyCount++;
    }
}

// For Leaf Node Deletion Only!
void Node::deleteKey(int key) {
    if (this->leafNode) {
        for (int i = 0; i < this->currKeyCount;i++) {
            if (this->keys[i] == key) {
                // If last key dont need do anything
                if (i != this->currKeyCount-1) {
                    // Not Last Key, need to shift stuff up
                    // Use currKeyCount-1 as we will be replacing curr key with next key, thus at
                    // currKeyCount-1 we will be replacing with the last key alr
                    for (int k=i;k < (this->currKeyCount-1);k++) {
                        this->keys[k] = this->keys[k+1];
                        this->pointers[k] = this->pointers[k+1];
                    }
                }
                this->currKeyCount--;
                break;
            }
        }
    }
}

// For removing child node, for internal nodes
void Node::deleteKey(int* ptr) {
    for (int i=0;i<=this->currKeyCount;i++) {
        if (this->pointers[i] == ptr) {
            if (i > 0) {
                // Move Keys and Pointers down
                for (int k=i;k < this->currKeyCount;k++) {
                    this->keys[k-1] = this->keys[k];
                    this->pointers[k] = this->pointers[k+1];
                }
            /*    1 < 3
                1,2 < Valid k values
                0 1 2 3 Original Pointers
            1   0 2 2 3
            2   0 2 3 3

                1 2 3 Original Keys
            1   2 2 3
            2   2 3 3 
                |1|2|3| */
            }else {
                // i == 0
                // Run Key count - 1 times as removing 1 key
                for (int k=i;k < this->currKeyCount-1;k++) {
                    this->keys[k] = this->keys[k+1];
                    this->pointers[k] = this->pointers[k+1];
                }
                // Have to move last pointer down as for loop doesnt cover last pointer in pointer list
                this->pointers[(this->currKeyCount-1)] = this->pointers[this->currKeyCount];
                /* 0 < (3-1) = 0 < 2
                0,1 < Valid k values
                
                0 1 2 3 Original Pointers
            0   1 1 2 3
            1   0 2 2 3
            currKey-1 = 2
                0 2 2 3
        currkey 0 2 3 3

                1 2 3 Original Keys
            0   2 2 3
            1   2 3 3 
                */
            }break;
        }
    }
    this->currKeyCount--;
}