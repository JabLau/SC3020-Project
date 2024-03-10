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

void BPTree::printTree(){
    // Check if root node is empty, if empty means tree is empty
    if (this->rootNode == nullptr) {
        // Tree is empty
        cout << "Tree is empty" << endl;
        return;
    }

    cout << "Printing B+ Tree" << endl;

    // Use queue to do BFS
    queue<Node*> fifoQueue;
    fifoQueue.push(this->rootNode);
    int level = 0;

    // Main loop to print tree
    while (!fifoQueue.empty()) {
        int nodesInLevel = fifoQueue.size();
        cout << "Level " << level << ":" << endl;

        for (int i=0; i<nodesInLevel; i++){
            Node* currNode = fifoQueue.front();
            fifoQueue.pop();

            // Print boarder
            cout << "[ ";
            // Print the contents of the node
            for (int j=0; j<currNode->currKeyCount; j++) {
                cout << currNode->keys[j] << " ";
            }
            // Print closing boarder
            cout << "] ";

            // enqueue all child nodes of an internal node
            if (!currNode->leafNode) {
                for (int j=0; j<=currNode->currKeyCount; j++){
                    fifoQueue.push((Node*) currNode->pointers[j]);
                }
            }
        }

        cout << endl;
        level++;
    }
}

void BPTree::returnNodeCount() {
        // Check if root node is empty, if empty means tree is empty
    if (this->rootNode == nullptr) {
        // Tree is empty
        cout << "Tree is empty" << endl;
        return;
    }

    // Use queue to do BFS
    queue<Node*> fifoQueue;
    fifoQueue.push(this->rootNode);
    int level = 0;
    int nodeCount = 0;
    // Main loop to print tree
    while (!fifoQueue.empty()) {
        int nodesInLevel = fifoQueue.size();

        for (int i=0; i<nodesInLevel; i++){
            Node* currNode = fifoQueue.front();
            fifoQueue.pop();
            nodeCount++;

            // enqueue all child nodes of an internal node
            if (!currNode->leafNode) {
                for (int j=0; j<=currNode->currKeyCount; j++){
                    fifoQueue.push((Node*) currNode->pointers[j]);
                }
            }
        }
        level++;
    }

    cout << "B+ Tree n:" << this->maxKeys << endl;
    cout << "B+ Tree Node Count:" << nodeCount << endl;
    cout << "B+ Tree Total Levels:" << level << endl;
    cout << "Root Node Content: [";
    Node* root = this->rootNode;
    for (int i=0;i < root->currKeyCount;i++) {
        cout << " " << root->keys[i];
    }
    cout << " ]" << endl;
}

bool BPTree::splitNode(Node* node, int key, int *address) {
    Node* splitNode = node->splitNode(key, address);
    Node* parentPtr = node->getParentPointer();
    if (parentPtr != nullptr) {
        if (parentPtr->canAddChild() == false) {
              this->splitNode(parentPtr, splitNode->getSelfLowerBoundKey(), (int*)splitNode);
        }else {
            // Add Child in
            parentPtr->addChild(splitNode->getSelfLowerBoundKey(), (int*) splitNode);
        }
    }else {
        // No parent ptr, need to create parent!
        parentPtr = new Node(this->maxKeys);
        parentPtr->addChild(0, (int*)node); //First Ptr so key doesnt matter
        parentPtr->addChild(splitNode->getSelfLowerBoundKey(), (int*) splitNode);
        // TODO: Darren help check if my understanding of this is correct
        // If im the root node and i split
        // THe new parent is the root node
        // Help me double confirm, that there wont be any subtrees that are stranded
        // Stranded = cannot access cause the subtree address wasnt tagged to the new parent!
        if (this->rootNode == node) {
            this->rootNode = parentPtr;
        }
    }
}

bool BPTree::insertKey(int key, int* address) {
    // Case 1: Tree Empty
    if (this->rootNode == nullptr) {
        Node *root = new Node(this->maxKeys);
        root->setLeafNode(true);
        vector<int*> *arr = new vector<int*>;
        root->addChild(key, (int *) arr);
        this->rootNode = root;
    } else {
        vector<int *> *findArr = this->findByValue(key,false);
        if (findArr != nullptr) {
            (*findArr).push_back(address);
        } else {
            Node *currNode = (this->rootNode);
            vector<int*> *arr = new vector<int*>;
            // Navigate to Leaf Node
            while (currNode->leafNode == false) {
                for (int i = 0; i < currNode->currKeyCount; i++) {
                    if (currNode->keys[i] > key) {
                        currNode = (Node *) currNode->pointers[i];
                        break;
                    }

                    if (i == currNode->currKeyCount - 1) {
                        // Set as last pointer of Internal Node
                        currNode = (Node *) currNode->pointers[currNode->currKeyCount];
                        break;
                    }
                }
            }
            arr->push_back(address);
            // Found Leaf Node, Check if can fit in new value
            if (currNode->canAddChild()) {
                currNode->addChild(key, (int *) arr);
            } else {
                // Node Full cannot add any new values
                // Need split
                this->splitNode(currNode, key, (int *) arr);
            }
        }
    }
    return true;
}

bool BPTree::bulkLoad(vector<tempStruct2> &list, int size) {

    int level = 0; // Track what level we are on curr
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
    int is500 = 0;
    // cout << level << "|" << totalNodes << "|" << currNode << endl;
    for (int i=0;i < size;i++) {
        if (list[i].key == 52) {
            is500++;
        }

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
        currNode->insertLeafNodeKey(list[i].key, (int*) &list[i].addresses);
    }

    cout << "500s:" << is500 << endl;
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
                navNode->setParentPointer(currNode);
                nodeFirstPtr = false;
            }else {
                // Add first key and pointer to node
                currNode->addChild(getLowerBoundOfSubTree(navNode), (int*)navNode);
                navNode->setParentPointer(currNode);
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

int BPTree::getLowerBoundOfSubTree(Node* curr) {
    while (curr->leafNode == false) {
        curr = (Node*) curr->pointers[0];
    }
    return curr->keys[0];
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

//void BPTree::printTree() {
//    Node* navNode = this->rootNode;
//    navNode->printNode();
//}

vector<int*>* BPTree::findByValue(int value, bool printAccessed) {
    int *indexNodesAccessed = new int();
    (*indexNodesAccessed) = 1;
    Node *currNode = this->findNodeWithValue(value,indexNodesAccessed);
    if (currNode == nullptr) {
        //cout << "Cannot find node with value" << endl;
        return nullptr;
    }

    double totalAvgRating = 0; 
    int recordCount = 0;

    bool overshot = false;
    Record* tempRecord;
    do {
        for (int i=0;i < currNode->currKeyCount;i++) {
            if (currNode->keys[i] == value) {
                if (printAccessed) {
                    cout << "B+ Tree Index nodes Accessed:" << (*indexNodesAccessed) << endl;
                }
                return ((vector<int*>*)currNode->pointers[i]);
            }else if (currNode->keys[i] > value) {
                overshot = true;
                break;
            }
        }

        if (overshot) {
            break;
        }
        currNode = (Node*) currNode->getNextNodePointer();
        (*indexNodesAccessed)++;
    }while (currNode != nullptr);

    return nullptr;
}

Node* BPTree::findNodeWithValue(int value, int* indexNodesAccessed) {
    Node* currNode = this->rootNode;
    bool foundNextNode;
    if (indexNodesAccessed == nullptr) {
        indexNodesAccessed = new int();
        (*indexNodesAccessed) = 1;
    }
    while (currNode->leafNode == false) {
        // Not leaf node
        foundNextNode = false;
        for (int i=0;i < currNode->currKeyCount;i++) { 
            if (value < currNode->keys[i]) {
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
        (*indexNodesAccessed)++;
    }

    // Value < Smallest value in B+Tree, cannot find
    // Value > Largest value in B+Tree, cannot find
    if (value < currNode->keys[0] || currNode->getNextNodePointer() == nullptr && value > currNode->keys[currNode->currKeyCount-1]) {
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

        if (currNode->keys[0] > value) {
            break; // Overshot liao, means cannot find
        }
        currNode = (Node*) currNode->getNextNodePointer(); // Go to next node
        (*indexNodesAccessed)++;
    }while (currNode != nullptr);
    // Terminate if found the value or
    // If reach end of line without finding value
    // Either by traversing all the nodes till the end w/o finding the value or
    // This the only node and does not have the value

    if (foundValue) {
        // Found value in this node, return it
        return currNode;
    }

    // Could not find value
    return nullptr;
}

vector<vector<int*>>* BPTree::findByRange(int start, int end,bool printAccessed) {
    // Implement checks to prevent end < start?
    int *indexNodesAccessed = new int();
    (*indexNodesAccessed) = 1;
    Node *currNode = this->findStartingNodeForRange(start, indexNodesAccessed);
    vector<vector<int*>> *defaultRtn = new vector<vector<int*>>;
    if (currNode == nullptr) {
        //cout << "Cannot find node with value" << endl;
        return nullptr;
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
                defaultRtn->push_back((*(vector<int*>*) currNode->pointers[i]));
//                tempRecord = (Record*) currNode->pointers[i];
//                totalAvgRating += tempRecord->avgRating;
//                recordCount++;
            }
        }

        if (overshot) {
            break;
        }
        currNode = (Node*) currNode->getNextNodePointer();
        (*indexNodesAccessed)++;
    }while (currNode != nullptr);

    if (printAccessed) {
        cout << "B+ Tree Index nodes Accessed:" << (*indexNodesAccessed) << endl;
    }
    return defaultRtn;
}

Node* BPTree::findStartingNodeForRange(int value,int *indexNodesAccessed) {
    Node* currNode = this->rootNode;
    bool foundNextNode;
    while (currNode->leafNode == false) {
        // Not leaf node
        foundNextNode = false;
        for (int i=0;i <= currNode->currKeyCount;i++) {
            if (value < currNode->keys[i]) {
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
        (*indexNodesAccessed)++;
    }

    // Value < Smallest value in B+Tree, cannot find
    // Value > Largest value in B+Tree, cannot find
    if (value < currNode->keys[0] || currNode->getNextNodePointer() == nullptr && value > currNode->keys[currNode->currKeyCount-1]) {
        return nullptr;
    }

    // Check if current leaf node has value
    // If not traverse nodes until find Node with Value
    bool foundValue = false;
    do {
        for (int i=0;i < currNode->currKeyCount;i++) {
            if (currNode->keys[i] >= value) {
                foundValue = true;
                break;
            }
        }

        if (foundValue) {
            break; // Found, now GTFO of loop
        }
        currNode = (Node*) currNode->getNextNodePointer(); // Go to next node
        (*indexNodesAccessed)++;
    }while (currNode != nullptr);
    // Terminate if found the value or
    // If reach end of line without finding value
    // Either by traversing all the nodes till the end w/o finding the value or
    // This the only node and does not have the value

    if (foundValue) {
        // Found value in this node, return it
        return currNode;
    }

    // Could not find value
    return nullptr;
}

bool BPTree::deleteNodes(int value) {
    Node* currNode;
    Node* parentNode;
    Node* leftNode;
    Node* rightNode;
    currNode = findNodeWithValue(value, nullptr);

    if (currNode == nullptr) {
        return false;
    }
    // At Leaf Node
    if (currNode != nullptr) {
        currNode->deleteKey(value);
        parentNode = currNode->getParentPointer();
        if (currNode->nodeValid() == false && parentNode != nullptr) {
            // Get Sibling Nodes
            int* tempCurrNodePtr = (int*) currNode; //Get int ptr ver of currNode for comparison
            leftNode = nullptr;
            rightNode = nullptr;
            for (int i=0;i <= parentNode->currKeyCount;i++) {
                if (parentNode->pointers[i] == tempCurrNodePtr) {
                    if (i > 0) {
                        leftNode = (Node*) parentNode->pointers[i-1]; 
                    }

                    if (i < parentNode->currKeyCount) {
                        rightNode = (Node*) parentNode->pointers[i+1]; 
                    }break;
                }
            }
            bool borrowed = false;
            tempStruct borrowedStruct;
            int* changed; //Node that value has been changed and parent need to update
            // Case 1: Borrow from Siblings
            if (leftNode != nullptr) {
                if (leftNode->borrowKeyCheck()) {
                    borrowedStruct = leftNode->borrowKeyCauseDeletion(true);
                    borrowed = true;
                    // Insert to front of currNode
                    currNode->addChild(borrowedStruct.key, borrowedStruct.address);
                    changed =(int*) currNode;
                }
            }

            if (rightNode != nullptr && !borrowed) {
                if (rightNode->borrowKeyCheck()) {
                    borrowedStruct = rightNode->borrowKeyCauseDeletion(false);
                    borrowed = true;
                    // Insert to back of currentNode
                    currNode->addChild(borrowedStruct.key,borrowedStruct.address);
                    changed =(int*) rightNode;
                }
            }
            
            // Update B+ Tree Parents for Borrowed Nodes
            // if (borrowed) {
            //     Node* navParent = ((Node*)changed)->getParentPointer();
            //     while (navParent != nullptr) {
            //         for (int i=0;i<navParent->currKeyCount;i++) {
            //             if (navParent->pointers[i] == changed) {
            //                 if (i > 0) {
            //                     navParent->keys[i-1] = getLowerBoundOfSubTree((Node*) changed);
            //                     navParent = nullptr;
            //                 }
            //                 break;
            //             }
            //         }
            //         if (navParent != nullptr) {
            //             changed =(int*) navParent;
            //             navParent = navParent->getParentPointer();
            //         }
            //     }
            // }

            if (!borrowed) {
                Node* removedNode;
                // Case 2: Merge with Siblings (Left if have, right if dont have)
                // If i previously tried to borrow from Left Node and failed,
                // Left node has enough space for me to merge
                if (leftNode != nullptr) {
                    leftNode->mergeNode(currNode);
                    removedNode = currNode;
                    leftNode->setNextNodePointer(currNode->getNextNodePointer());
                }else if (rightNode == nullptr) {
                    cout << "yo wtf going on here?" << endl;
                }else{
                    // Merge with right
                    // Do checks if there is a right node? but if this fails how?
                    currNode->mergeNode(rightNode);
                    removedNode = rightNode;
                    currNode->setNextNodePointer(rightNode->getNextNodePointer());
                }
                
                Node* navParent = removedNode->getParentPointer();
                int* updatedNode;
                while (navParent != nullptr) {
                    if (removedNode != nullptr) {
                        navParent->deleteKey((int*) removedNode);
                        free(removedNode);
                        removedNode = nullptr;

                        if (navParent->nodeValid() == false) {
                            // Check  borrow?
                            parentNode = navParent->getParentPointer();

                            if (parentNode == nullptr) {
                                // No Parent is root node!
                                cout << "Root node not valid" << endl;
                                // If root only has 1 child node, child node is new root node
                                if (navParent->currKeyCount == 0) {
                                    Node* newRoot = (Node*)navParent->pointers[0];
                                    this->rootNode = newRoot;
                                    newRoot->removeParentPointer();
                                    free(navParent);
                                    navParent = nullptr;
                                }
                                break;
                            }

                            int* tempCurrNodePtr = (int*) navParent; //Get int ptr ver of navParent for comparison
                            leftNode = nullptr;
                            rightNode = nullptr;
                            for (int i=0;i <= parentNode->currKeyCount;i++) {
                                if (parentNode->pointers[i] == tempCurrNodePtr) {
                                    if (i > 0) {
                                        leftNode = (Node*) parentNode->pointers[i-1];
                                    }

                                    if (i < parentNode->currKeyCount) {
                                        rightNode = (Node*) parentNode->pointers[i+1]; 
                                    }break;
                                }
                            }
                            bool borrowed = false;

                            if (leftNode != nullptr) {
                                if (leftNode->borrowKeyCheck()) {
                                    borrowedStruct = leftNode->borrowKeyCauseDeletion(true);
                                    borrowed = true;
                                    // Insert to back of currentNode
                                    navParent->addChild(borrowedStruct.key,borrowedStruct.address);
                                    // Update child node parent
                                    Node* childNode = (Node*) borrowedStruct.address;
                                    childNode->setParentPointer(navParent);
                                    updatedNode =(int*) navParent;
                                }
                            }

                            if (rightNode != nullptr && !borrowed) {
                                if (rightNode->borrowKeyCheck()) {
                                    borrowedStruct = rightNode->borrowKeyCauseDeletion(false);
                                    borrowed = true;
                                    // Insert to back of currentNode
                                    navParent->addChild(borrowedStruct.key,borrowedStruct.address);
                                    // Update child node parent
                                    Node* childNode = (Node*) borrowedStruct.address;
                                    childNode->setParentPointer(navParent);
                                    updatedNode =(int*) rightNode;
                                }
                            }

                            if (borrowed) {
                                navParent = navParent->getParentPointer();
                            }

                            // Must merge?
                            if (!borrowed) {
                                if (leftNode != nullptr) {
                                    leftNode->mergeNode(navParent);
                                    removedNode = navParent;
                                }else {
                                    navParent->mergeNode(rightNode);
                                    removedNode = rightNode;
                                }
                                navParent = removedNode->getParentPointer();
                            }
                        }else {
                            updatedNode = (int*) navParent;
                            navParent = navParent->getParentPointer();
                        }
                    }else {
                        // Not merged, changed only
                        for (int i=0;i<navParent->currKeyCount;i++) {
                            if (navParent->pointers[i] == updatedNode) {
                                if (i > 0) {
                                    navParent->keys[i-1] = getLowerBoundOfSubTree((Node*) updatedNode);
                                    navParent = nullptr;
                                }
                                break;
                            }
                        }
                        if (navParent != nullptr) {
                            updatedNode =(int*) navParent;
                            navParent = navParent->getParentPointer();
                        }
                    }
                }
            }
        }
    }
    return true;
}

void BPTree::leafNodeCheck() {
    Node* curr = this->rootNode;
    int largestVal= 0;
    while (!curr->leafNode) {
        curr = (Node*) curr->pointers[0];
    }

    while (curr != nullptr) {
        for (int i=0; i < curr->currKeyCount;i++) {
            if (curr->keys[i] > largestVal) {
                largestVal = curr->keys[i];
            }else {
                cout << "****Leaf check FAILED FAILED FAILED!!!!****" << endl;
                return;
            }
        }
        curr = (Node*) curr->getNextNodePointer();
    }
    cout << "Leaf node check correct" << endl;
    cout << "Largest Value =" << largestVal << endl;
    return;
}