#pragma once
#define DiskManager_H
#include "record.h"
#include <queue>

class DiskManager
{
public:
	int currBlock = 0;
	int currBlockMemUsed = 0;
	int totalBlocks;
	int blockSize;
	int availableBlocks;
	int maxRecords;
	int totalRecords = 0;

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
};

