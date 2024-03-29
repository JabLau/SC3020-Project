#pragma once
#define DiskManager_H
#include "record.h"
#include "tempStruct.h"
#include <queue>
#include <vector>

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

	addressInfo storeRecord(Record *r);
    int *getBlockAddress(int blockId);
    void printAllRecords();
    addressInfo getNextAvailableAddress();
	std::vector<tempStruct> addr_NumVotes_arr();

    int tabulateBlockUsage(vector<tempStruct> recordAddresses);

    int getBlockId(int *address);

    bool checkExist(vector<int> temp, int key);

    int tabulateBlockUsage(vector<int *> recordAddresses);

    int tabulateBlockUsageNested(vector<vector<int *>> recordAddresses);

    Record getRecord(int *recordAddress);

    vector<int *> *searchAllRecords(int numVotesKey);

    vector<int *> *searchAllRecordsRange(int numVotesStartRange, int numVotesEndRange);

    void searchAllRecordsDeletion(int numVotesKey);
};

