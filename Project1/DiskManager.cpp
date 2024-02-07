#include "DiskManager.h"
#include <cstdlib>
#include "record.h"

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

bool DiskManager::storeRecord(Record r) {
	// Check if any memory available
	if (totalRecords >= maxRecords ) {
		return false;
	}

	int* address;

	if (this->deletedRecords.empty() == false) {
		int recordNoDeleted = this->deletedRecords.front();
		this->deletedRecords.pop();

		address = this->memStartAddress + ((recordNoDeleted - 1) * sizeof(Record));

		memcpy(address, &r, sizeof(Record));
	}else {
		// Store Record
		address = currBlockPointer();
		memcpy(address, &r, sizeof(Record));
		this->currBlockMemUsed += sizeof(Record);

		// Check if in next block
		if (this->currBlockMemUsed > this->blockSize) {
			this->currBlockMemUsed -= this->blockSize;
			this->currBlock++;
		}
	}
	this->totalRecords++;
	return true;
}

Record DiskManager::getRecord(int recordNo) {
	if (recordNo > totalRecords) {
		return Record();
	}

	Record* recordPointer = (Record*)(this->memStartAddress + ((recordNo - 1) * sizeof(Record)));
	Record rtnRecord = Record(recordPointer->movieId, recordPointer->avgRating, recordPointer->numVotes);
	
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