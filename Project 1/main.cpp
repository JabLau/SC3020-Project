// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Spanned
// Non-Sequential - Because id are not in order
// Fixed format with fixed length
// Block size = 200b
// Total memory size = 250MB
// Total number of blocks = 250MB / 200b = 1,250,000


#include <iostream>
#include <cmath>
#include <queue>
#include "Record.h"
#include "DiskManager.h"
#include "FileManager.h"
#include "Node.h"
#include "BPTree.h"
#include "tempStruct.h"
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;

void printMem(int* addr, int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%08x ", addr[i]);
    printf("\n\n");
}

vector<tempStruct> experiment_1(DiskManager* disk){
    // Load the data from disk
    FileManager fm = FileManager();
    vector<tempStruct> unsorted_list = fm.load_data(disk);
    std::cout << unsorted_list.size() <<" "<< endl;
    int dashLength = 20;
    // Print leading dashes
    for (int i = 0; i < dashLength; ++i) {
        std::cout << "-";
    }
    std::cout << " Experiment 1 ";
    // Print trailing dashes
    for (int i = 0; i < dashLength; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;

    //• the number of records;
    cout << "The number of records: " << fm.recordCount << endl;
    //• the size of a record;
    cout << "The size of a record: " << sizeof(Record) << endl;
    //• the number of records stored in a block;
    cout << "The number of records stored in a block: " << disk->blockSize / sizeof(Record) << endl;
    //• the number of blocks for storing the data;
    cout << "The number of blocks for storing the data: " << fm.blockCount << endl;

    return unsorted_list;
}

void experiment_2(BPTree* bt){
    cout<<"Experiment 2"<<endl;
    bt->returnNodeCount();
}

void experiment_3(BPTree* bt, DiskManager *disk){
    cout<<"Experiment 3"<<endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    vector<int*> *addressList = bt->findByValue(500, true);
    auto endTime = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    int blocksUsed = disk->tabulateBlockUsage(*addressList);
    cout << "Data blocks accessed (Index): " << blocksUsed << endl;
    int count = 0;
    float sum=0.0;
    for (int* addr: *addressList){
        // Get record
        Record r;
        r = disk->getRecord(addr);
        count++;
        sum+= r.avgRating;
    }
    float avgRatings = 0.0;
    avgRatings = sum / count;
    cout << "Average Ratings for records 500: " << avgRatings << endl;
    cout << "Running time of retrieval process: " << duration << " milliseconds." << endl;
    // Sequential or linear scan
    disk->searchAllRecords(500);
}

void experiment_4(BPTree* bt, DiskManager *disk){
    cout<<"Experiment 4"<<endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    vector<vector<int*>> *addressList = bt->findByRange(30000,40000, true);
    auto endTime = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    // Calculate average of records in addressList
    int count=0;
    float sum=0.0;
    int blocksUsed=0;
    for (vector<int*> vec : *addressList){
        blocksUsed += disk->tabulateBlockUsage(vec);
        for (int* addr: vec){
            // Get record
            Record r;
            r = disk->getRecord(addr);
            count++;
            sum+= r.avgRating;
        }
    }
    float avgRatings = 0.0;
    avgRatings = sum / count;
    cout << "Data block accessed (Index): " << blocksUsed << endl;
    cout << "Average Ratings for records in between 30k - 40k: " << avgRatings << endl;
    cout << "Running time of retrieval process: " << duration << " milliseconds." << endl;
    disk->searchAllRecordsRange(30000, 40000);
}

void experiment_5(BPTree* bt, DiskManager *disk){
    cout<<"Experiment 5"<<endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    bt->deleteNodes(1000);
    auto endTime = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    bt->returnNodeCount();
    cout << "Running time of deletion process: " << duration << " milliseconds." << endl;
    disk->searchAllRecordsDeletion(1000);
}

void testBPTree2() {
    int blockSize = 200; // in bytes
    int totalMemSize = 250 * 1000000; //in MB

    // Allocate space simulating main memory
    DiskManager disk = DiskManager(blockSize, totalMemSize);

    BPTree bt = BPTree(3);
    addressInfo addr;
    int *absol_addr;
    cout << "Inserting" << endl;
    int test[] = {50,53,70,76,90,100,83,86,77,56};
    for (int i = 0; i < 150; i++) {
        int randInt = rand() % 99999;
        cout << randInt << endl;
        Record *r1 =new  Record(to_string(randInt), to_string(5.5), to_string(randInt));
        addr = disk.storeRecord( r1);
        absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
        bt.insertKey(randInt, absol_addr);
    }
//    bt.printTree();
//    bt.leafNodeCheck();
//
    // for (int i=0; i < 10;i++) {
    //     Record r1 = Record(to_string(test[i]), to_string(5.5), to_string(test[i]));
    //     addr = disk.storeRecord(r1);
    //     absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    //     bt.insertKey(test[i], absol_addr);
    //     bt.printTree();
    // }
    // for (int i=1; i < 6;i++) {
    //     Record r1 = Record(to_string(i*10), to_string(5.5), to_string(i*10));
    //     addr = disk.storeRecord(r1);
    //     absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    //     bt.insertKey(i*10, absol_addr);
    //     bt.printTree();
    // }
    // Record r1 = Record(to_string(45), to_string(5.5), to_string(45));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(45, absol_addr);
    // Record r1 = Record(to_string(75), to_string(5.5), to_string(75));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(75, absol_addr);
    // bt.printTree();
}


void testBPTree(vector<tempStruct> &arr){
    // Settings
    int blockSize = 200; // in bytes
    int totalMemSize = 250 * 1000000; //in MB

    // Allocate space simulating main memory
    DiskManager disk = DiskManager(blockSize, totalMemSize);

    BPTree bt = BPTree(3);
    //const int listSize = 25;
    //tempStruct list[listSize];
    
    // addressInfo addr;
    // int *absol_addr;
    // for (int i=0; i < listSize;i++) {
    //     // Record r1 = Record(to_string(123+i), to_string(i+0.5), to_string(99+floor(i/2))); // For testing duplicate key values
    //     Record r1 = Record(to_string(123+i), to_string(i+0.5), to_string(99+i));
    //     addr = disk.storeRecord(r1);
    //     absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    //     cout << r1.numVotes << "|" << absol_addr << endl;
    //     list[i].key = r1.numVotes;
    //     list[i].address = absol_addr;
    // }
    //bt.bulkLoad(arr, arr.size());
    //bt.findByRange(100,101);
}


void heapify(vector<tempStruct> &arr, int N, int i)
{

    // Initialize largest as root
    int largest = i;

    // left = 2*i + 1
    int l = 2 * i + 1;

    // right = 2*i + 2
    int r = 2 * i + 2;

    // If left child is larger than root
    if (l < N && arr[l].key > arr[largest].key)
        largest = l;

    // If right child is larger than largest
    // so far
    if (r < N && arr[r].key> arr[largest].key)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected
        // sub-tree
        heapify(arr, N, largest);
    }
}

// Main function to do heap sort
void heapSort(vector<tempStruct> &arr, int N)
{

    // Build heap (rearrange array)
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(arr, N, i);

    // One by one extract an element
    // from heap
    for (int i = N - 1; i > 0; i--) {

        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

void bubbleSort(vector<tempStruct> &arr, int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j].key > arr[j + 1].key) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}

int main()
{
    //testBPTree();
    // testBPTree2();
    // return 1;
    // Settings
    int blockSize = 200; // in bytes
    int totalMemSize = 250 * 1000000; //in MB

    // Allocate space simulating main memory
    DiskManager disk = DiskManager(blockSize, totalMemSize);
    
    // Experiment 1
    vector<tempStruct> unsorted_list = experiment_1(&disk);
    BPTree bt = BPTree(16);
    int listSize = unsorted_list.size();
    for (int i=0; i < listSize;i++) {
        // cout << "Adding" << unsorted_list[i].key << endl;
        bt.insertKey(unsorted_list[i].key, unsorted_list[i].address);
        //bt.printTree();
    }
    cout <<"Insertion Done" << endl;
    experiment_2(&bt);
    experiment_3(&bt, &disk);
    experiment_4(&bt, &disk);
    experiment_5(&bt, &disk);
    //bt.printTree();
    // int n=unsorted_list.size();
    // cout <<"Length of Unsorted List = "<<n <<endl;
    // heapSort(unsorted_list, n);
    // //bubbleSort(unsorted_list, n);
    // cout <<"Sort Complete" << endl;


    // // new vector
    // vector<tempStruct2> *uniqueArr = new vector<tempStruct2>;
    // int j=0;

    // // Allocate into unique linked list
    // for (int i=0;i < n; i++) {
    //     // Check if uniqueArr is empty
    //     if (uniqueArr->empty() || (*uniqueArr)[j-1].key != unsorted_list[i].key){
    //         // Create a tempStruct2
    //         tempStruct2 temp;
    //         temp.key = unsorted_list[i].key;
    //         // temp.addresses.push_back(unsorted_list[i].address);
    //         // Put into uniqueArr
    //         uniqueArr->push_back(temp);
    //         j++;
    //     } else {
    //         (*uniqueArr)[j-1].addresses.push_back(unsorted_list[i].address);
    //     }
    // }
    // // system("pause");
    // cout << "Check if all can find" << endl;
    // int s = (*uniqueArr).size();
    // for (int i=0;i < s;i++) {
    //     if(bt.findByValue((*uniqueArr).at(i).key, false) == nullptr) {
    //         cout << "Key " << (*uniqueArr).at(i).key << " missing?" << endl;
    //     }
    // }
    // addressInfo addr;
    // int *absol_addr;
    // Record r1 = Record(to_string(75), to_string(5.5), to_string(75));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(75, absol_addr);
    // r1 = Record(to_string(75), to_string(5.5), to_string(75));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(75, absol_addr);
    // r1 = Record(to_string(75), to_string(5.5), to_string(75));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(73, absol_addr);
    // r1 = Record(to_string(75), to_string(5.5), to_string(75));
    // addr = disk.storeRecord(r1);
    // absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
    // bt.insertKey(75, absol_addr);
    // bt.printTree();
    // bt.printTree();
    // bt.leafNodeCheck();
    // bt.bulkLoad(*uniqueArr, uniqueArr->size());
//    cout << "Retrieve numVotes = 500" << endl;
//    vector<int*> find500 = bt.findByValue(500);
//    vector<vector<int*>> findRangeVal = bt.findByRange(30000,40000);

//    cout << "Retrieve 30,000 <= numVotes <= 40,000" << endl;
//    bt.findByRange(30000,40000);
//    // cout << "Retrieve numVotes = 1000" << endl;
//    // bt.findByValue(1000);
//    bool complete = true;
//    int counter = 0;
//    do {
//        counter++;
//        complete = bt.deleteNodes(52);
//    }while (complete);
//    counter--; //Last run nvr delete
//    cout << "Deleted 52: " << counter << endl;
    // for (int i=0;i < 100000; i++) {
    //     cout << i << ":" << unsorted_list[i].key << ",";
    // }
    // cout << endl;

//    // Loop and print each memory byte
//    int* startAddr = disk.memStartAddress;
//    // Print start memory
//    cout << "Start memory: " << disk.memStartAddress << endl;
//    for (int i = 0; i < blockSize; i++) {
//        cout << "Address: " << static_cast<void*>(startAddr + i) << ", Value: " << static_cast<unsigned int>(*(startAddr + i)) << endl;
//    }
//
//    // Read all records
//    disk.printAllRecords();

    // Experiment 2 (TODO)
//    Record r1 = Record("123", "2.5", "100");
//    Record r2 = Record("456", "2.5", "105");
//    Record r3 = Record("789", "2.5", "99");
//    int* addr1 = disk.storeRecord(r1);
//    int* addr2 = disk.storeRecord(r2);
//    int* addr3 = disk.storeRecord(r3);
//    Node n = Node(3);
//    n.addKey(r1.numVotes, addr1);
//    n.addKey(r2.numVotes, addr2);
//    n.addKey(r3.numVotes, addr3);
//    n.printNode();
//
//    BPTree bt = BPTree(3);
//    bt.insertKey(r1.numVotes,addr1);
//    bt.insertKey(r2.numVotes,addr2);
//    bt.insertKey(r3.numVotes,addr3);
//    // Read file and populate allocated area
//    FileManager fm = FileManager();
//    // fm.load_data(disk);
//
//    // Loop and print each memory byte
//    int* startAddr = disk.memStartAddress;
//    // Print start memory
//    cout << "Start memory: " << disk.memStartAddress << endl;
//    // Print memory contents of the record after storing
//    // for (int i = 0; i < blockSize; i++) {
//    //     cout << "Address: " << (startAddr + i) << ", Value: " << *(startAddr + i) << endl;
//    // }
//
//    // Read all records
//    disk.printAllRecords();

//    int* addr = disk.getStartAddress();
//    Record r = disk.getRecord(addr);
//    r.printRecord();
    // Print record
    //for (int i = 1; i <= 10; i++) {
    //    Record r = disk.getRecord(i);
    //    r.printRecord();
    //}

//    cout << disk.getStartAddress() << endl;

//    int recordSize = sizeof(Record);
//    int* startAddr = disk.currBlockPointer();
//    int i;
//    printMem(startAddr, recordSize);
//    disk.storeRecord(Record("TestTest", 1.5, 32));
//    printMem(startAddr, recordSize);
//    disk.storeRecord(Record("Bobby", 1.5, 100));
//    printMem(startAddr, 92);
//    disk.removeRecord(2);
//    disk.storeRecord(Record("HiAdrian", 1.5, 100));
//    Record r = disk.getRecord(1);
//    r.printRecord();
//    r = disk.getRecord(2);
//    r.printRecord();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
