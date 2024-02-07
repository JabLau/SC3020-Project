#include "DiskManager.h"
#include <cstdlib>
#include "record.h"
#include <iostream>

using namespace std;

DiskManager::DiskManager(int blockSize, int totalSize) {
	this->memStartAddress = (int*)malloc(totalSize);
	this->totalBlocks = totalSize / blockSize;
	this->blockSize = blockSize;
	this->maxRecords = totalSize / sizeof(Record);
}

DiskManager::~DiskManager() {
	free(this->memStartAddress);
}

int* DiskManager::storeRecord(Record r) {
	// Check if any memory available
	if (totalRecords >= maxRecords ) {
		return nullptr;
	}
	int* address;
	int* rtnAddress;

	// TODO: Redo Deleted Records Code
	if (this->deletedRecords.empty() == false) {
		int recordNoDeleted = this->deletedRecords.front();
		this->deletedRecords.pop();

		address = this->memStartAddress + ((recordNoDeleted - 1) * sizeof(Record));

		memcpy(address, &r, sizeof(Record));
	}else {
		int remainingSpace = this->blockSize - this->currBlockMemUsed;
		// Check current block enough to store nextSpanPtr and nextSpanLen
		if (remainingSpace < r.minimumSpace()) {
			this->currBlock++;
			this->currBlockMemUsed = 0;
			remainingSpace = this->blockSize;
		}

		// Store Record
		address = currBlockPointer();
		rtnAddress = address;

		int cpyLen = sizeof(Record);
		bool splitRecord = false;
        cout << "Remaining Space: " << remainingSpace << endl;
        cout << "Copy Length: " << cpyLen << endl;
		if (remainingSpace < cpyLen) {
			cpyLen = remainingSpace;
			splitRecord = true;
		}

		// If Splitting Record to 2 Blocks, need to store next block ptr
		if (splitRecord) {
			int* nextSpanAddr = address + cpyLen;
			r.setNextSpanAddress(nextSpanAddr, (sizeof(Record) - cpyLen));
            cout << "Next Span Address: " << nextSpanAddr << endl;
		}
		else {
			r.setNextSpanAddress(nullptr, 0);
		}

        memcpy(address, &r, cpyLen);
        if (splitRecord) {
            this->currBlock++;
            this->currBlockMemUsed = 0;
            address = currBlockPointer();
            memcpy(address, (&r + cpyLen), (sizeof(Record) - cpyLen));
            this->currBlockMemUsed = (sizeof(Record)-cpyLen);
        }
        else {
            this->currBlockMemUsed += sizeof(Record);
        }
	}
	this->totalRecords++;
	return rtnAddress;
}

Record DiskManager::getRecord(int recordNo) {
	if (recordNo > totalRecords) {
		return Record();
	}

	Record* recordPointer = (Record*)(this->memStartAddress + ((recordNo - 1) * sizeof(Record)));
	Record rtnRecord = Record(recordPointer->movieId, recordPointer->avgRating, recordPointer->numVotes);
	
	return rtnRecord;
}

Record DiskManager::getRecord(int* recordAddr) {
	int* nextSpanAddr;
	int nextSpanLen;
	memcpy(&nextSpanAddr, recordAddr, sizeof(int*));
	memcpy(&nextSpanLen, (recordAddr + sizeof(int*)), sizeof(int));

	Record rtnRecord;
	Record* recordPointer;

    if (false){
//	if (nextSpanAddr != nullptr) {
		int currSize = (sizeof(Record) - nextSpanLen);
		recordPointer = (Record*) malloc(sizeof(Record));
		memcpy(&recordPointer, recordAddr, currSize);
		memcpy(&recordPointer + currSize, nextSpanAddr, nextSpanLen);
		rtnRecord = Record(recordPointer->movieId, recordPointer->avgRating, recordPointer->numVotes);
	}
	else {
		recordPointer = (Record*)recordAddr;
		rtnRecord = Record(recordPointer->movieId, recordPointer->avgRating, recordPointer->numVotes);
	}
	return rtnRecord;
}

bool DiskManager::removeRecord(int recordNo) {
	this->deletedRecords.push(recordNo);
	return true;
}

int* DiskManager::currBlockPointer() {
	return this->memStartAddress + this->currBlock * this->blockSize + this->currBlockMemUsed;
}

int* DiskManager::getStartAddress() {
	return this->memStartAddress;
}