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
	int currBlock = 0;
	int currBlockMemUsed = 0;
	int totalNumBlocks;
	int blockSize;
	int availableBlocks;
	int maxRecords;
	int totalRecords = 0;
    int totalSize;
    int *freeSpacePerBlock;

	queue<int> deletedRecords;

	int* memStartAddress;

	DiskManager(int blockSize, int totalSize);
	~DiskManager();

	int* storeRecord(Record r);
	Record getRecord(int recordNo);
	Record getRecord(int*);
	bool removeRecord(int recordNo);

	int* currBlockPointer();
	int* getStartAddress();


    int *getBlockAddress(int blockId);
    addressInfo getNextAvailableAddress(Record r);

    addressInfo getNextSpanAddress(int recordId, int spillOverSize);

    void printAllRecords();

    addressInfo getNextAvailableAddress();
};

