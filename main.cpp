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



using namespace std;

void printMem(int* addr, int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%08x ", addr[i]);
    printf("\n\n");
}

void experiment_1(DiskManager disk){
    // Load the data from disk
    FileManager fm = FileManager();
    fm.load_data(disk);

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
    cout << "The number of records stored in a block: " << disk.blockSize / sizeof(Record) << endl;
    //• the number of blocks for storing the data;
    cout << "The number of blocks for storing the data: " << fm.blockCount << endl;
}

void testBPTree(){
    // Settings
    int blockSize = 200; // in bytes
    int totalMemSize = 250 * 1000000; //in MB

    // Allocate space simulating main memory
    DiskManager disk = DiskManager(blockSize, totalMemSize);

    BPTree bt = BPTree(3);
    const int listSize = 25;
    tempStruct list[listSize];

    addressInfo addr;
    int *absol_addr;
    for (int i=0; i < listSize;i++) {
        // Record r1 = Record(to_string(123+i), to_string(i+0.5), to_string(99+floor(i/2))); // For testing duplicate key values
        Record r1 = Record(to_string(123+i), to_string(i+0.5), to_string(99+i));
        addr = disk.storeRecord(r1);
        absol_addr = disk.getBlockAddress(addr.blockId)+addr.offset;
        cout << r1.numVotes << "|" << absol_addr << endl;
        list[i].key = r1.numVotes;
        list[i].address = absol_addr;
    }
    bt.bulkLoad(list, listSize);
    bt.findByRange(100,101);
}

int main()
{
    //testBPTree();

    // Settings
    int blockSize = 200; // in bytes
    int totalMemSize = 250 * 1000000; //in MB

    // Allocate space simulating main memory
    DiskManager disk = DiskManager(blockSize, totalMemSize);

    // Experiment 1
    experiment_1(disk);

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
