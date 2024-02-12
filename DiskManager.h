#pragma once
#define DiskManager_H
#include "record.h"
#include <queue>

// Struct of full address info
struct addressInfo {
    int blockId;
    int offset;
};

class DiskManager
{
public:
	int totalNumBlocks;
	int blockSize;
	int maxRecords;
	int totalRecords = 0;
    int totalSize;
    int *freeSpacePerBlock;

	queue<int> deletedRecords;

	int* memStartAddress;

	DiskManager(int blockSize, int totalSize);
	~DiskManager();

	addressInfo storeRecord(Record r);
    int *getBlockAddress(int blockId);
    void printAllRecords();
    addressInfo getNextAvailableAddress();
};

