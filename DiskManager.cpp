#include "DiskManager.h"
#include <cstdlib>
#include "record.h"
#include <iostream>
#include <cstring>
#include <vector>
#include "tempStruct.h"

using namespace std;

DiskManager::DiskManager(int blockSize, int totalSize) {
    this->blockSize = blockSize;
    this->totalSize = totalSize;
    this->totalNumBlocks = totalSize / blockSize;
    this->maxRecords = totalSize / sizeof(Record);

    // Allocate whole memory space simulating main memory blocks
	this->memStartAddress = (int*)malloc(totalSize);
    // Set malloc'd memory to -1
    memset(this->memStartAddress, -1, totalSize);

    // Array to keep track of free space in each block in bytes
    this->freeSpacePerBlock = (int*)malloc(this->totalNumBlocks * sizeof(int));
    // Set all blocks to be free initially
    for (int i = 0; i < this->totalNumBlocks; i++) {
        this->freeSpacePerBlock[i] = this->blockSize;
    }
}

// Destructor
DiskManager::~DiskManager() {
    cout << "Freeing Disk Manager" << this->memStartAddress << endl;
	free(this->memStartAddress);
}

// Method to calculate block starting address by block id
int* DiskManager::getBlockAddress(int blockId) {
    // 4 byte addressible memory
    return this->memStartAddress + (blockId * (this->blockSize/4));
    }

int DiskManager::getBlockId(int* address) {
    return (address - this->memStartAddress) / (this->blockSize/4);
}

bool DiskManager::checkExist(vector<int> temp, int key){
    for (int i : temp){
        if (i == key){
            return true;
        }
    }
    return false;
}

// Method to tabulate vector of record addresses into blocks used
int DiskManager::tabulateBlockUsage(vector<int*> recordAddresses) {
    // Vector to store block ids
    vector<int> blockIds;

    // Loop thru vector of addresses
    for (int i=0; i<recordAddresses.size(); i++) {
        // Convert address to block id
        int blockId = getBlockId(recordAddresses[i]);
        if (checkExist(blockIds, blockId)){
            continue;
        }
        else{
            blockIds.push_back(blockId);
        }
    }

    return blockIds.size();
}

int DiskManager::tabulateBlockUsageNested(vector<vector<int*>> recordAddresses) {
    vector<int> blockIds;

    // 2 loops
    // Loop thru vector of vectors
    for (int i=0; i<recordAddresses.size(); i++) {
        for (int k=0; k<recordAddresses[i].size(); k++) {
            // Convert address to block id
            int blockId = getBlockId((recordAddresses[i][k]));
            if (checkExist(blockIds, blockId)) {
                continue;
            }else{
                blockIds.push_back(blockId);
            }
        }

        return blockIds.size();
}


// Method to get next available memory address
addressInfo DiskManager::getNextAvailableAddress() {
    // Loop through freeSpacePerBlock array to find the first block with at least 1 byte of free space
    for (int i = 0; i < this->totalNumBlocks; i++) {
            if (this->freeSpacePerBlock[i] >= 1) {
                // Iterate through block, finding the address of the first byte that is -1
                for (int j = 0; j < this->blockSize/4; j++) {
                    if (*(this->getBlockAddress(i) + j) == -1) {
                        // Return address struct addressinfo, offset is in 4 byte addressible memory
                        return {i, j};

                    }
                }
            }
        }
    // If no available address, return nullptr
    return {-1, -1};
}

addressInfo DiskManager::storeRecord(Record r) {
	// Check if any memory available
	if (totalRecords >= maxRecords ) {
		return {-1, -1};
	}

    // Find the next available address
    addressInfo nextAvailableAddress = getNextAvailableAddress();
    // Print next available address
    //cout << "Found empty slot at: " << nextAvailableAddress.blockId << ", " << nextAvailableAddress.offset << endl;

    // Case 1: Address is -1 -1, no available space
    if (nextAvailableAddress.blockId == -1 && nextAvailableAddress.offset == -1) {
        // Print out error message and raise exception
        cout << "No available space" << endl;
        return {-1, -1};
    }

    // Case 2: Address has the space to accommodate the whole record, no spanning needed
    if (blockSize - ((getBlockAddress(nextAvailableAddress.blockId)+ nextAvailableAddress.offset)- getBlockAddress(nextAvailableAddress.blockId)) >= sizeof(Record)) {
        // Enough space to store the record without spanning
        memcpy(getBlockAddress(nextAvailableAddress.blockId) + nextAvailableAddress.offset, &r, sizeof(Record));
        // Update array to keep track of free space in each block in bytes
        this->freeSpacePerBlock[nextAvailableAddress.blockId] -= sizeof(Record);
    }
    // Case 3: Block only has partial space to accommodate the header and some of the record, spanning needed
    else {
        // Size left on current block = freeSpacePerBlock[nextAvailableAddress.blockId]
        int sizeLeft = freeSpacePerBlock[nextAvailableAddress.blockId]; // in bytes
        // Calculate the spillOverSize needed to store the record for next block
        int spillOverSize = sizeof(Record) - sizeLeft; // in bytes

        // Copy first half of the record to the original block
        cout << "Size left: " << sizeLeft << ", Spill over size: " << spillOverSize << endl;
        memcpy(getBlockAddress(nextAvailableAddress.blockId) + nextAvailableAddress.offset, &r, sizeLeft);
        // Update array to keep track of free space in each block in bytes
        this->freeSpacePerBlock[nextAvailableAddress.blockId] -= sizeLeft;

        memcpy(getBlockAddress(nextAvailableAddress.blockId+1), reinterpret_cast<char*>(&r) + sizeLeft, spillOverSize);
        // memcpy(getBlockAddress(nextAvailableAddress.blockId+1), (&r)+(sizeLeft/4), spillOverSize);
        // Update array to keep track of free space in each block in bytes
        this->freeSpacePerBlock[nextAvailableAddress.blockId+1] -= spillOverSize;
    }

    // Print relative address and return the absolute address of the record
    // cout << "Record inserted on block id, offset: " << nextAvailableAddress.blockId << ", " << nextAvailableAddress.offset << endl;
    return {nextAvailableAddress.blockId, nextAvailableAddress.offset};
    //cout << "Record inserted on block id, offset: " << nextAvailableAddress.blockId << ", " << nextAvailableAddress.offset << endl;
    //return getBlockAddress(nextAvailableAddress.blockId) + nextAvailableAddress.offset;
}

// Sequentially read the records from the memory
void DiskManager::printAllRecords() {
    Record r;
    // Loop through blocks that have at least 1 record
    for (int i = 0; i < this->totalNumBlocks; i++) {
        // If block has at least 1 record, ignoring spillover records (Read from original block only)
        if ((this->blockSize - this->freeSpacePerBlock[i]) >= sizeof(Record)) {
            // Loop through the block
            for (int j = 0; j < this->blockSize / 4; j++) {
                // If the byte is not -1, it is the start of a record
                if (*(this->getBlockAddress(i) + j) != -1) {
                    // Read record
                    if (j * 4 + sizeof(Record) > this->blockSize) {
                        // Case 1: Record is spanned, memcpy from this block and the next block to form a full record
                        // Copy first part of the record (On this block)
                        // Calculate remaining size of the record
                        int sizeLeft = (this->blockSize - j * 4);
                        int spillOverSize = sizeof(Record) - sizeLeft;
                        // Copy the first part of the record (On this block)
                        memcpy(&r, this->getBlockAddress(i) + j, sizeLeft);
                        // Copy the second part of the record (On the next block)
                        memcpy(reinterpret_cast<char*>(&r) + sizeLeft, this->getBlockAddress(i + 1), spillOverSize);
                        //memcpy(&r + sizeLeft, this->getBlockAddress(i + 1), spillOverSize);

                        // Print spanned
                        cout << "Spanned record: ";
                        r.printRecord();

                        // Skip to the next block and ignore spillOverSize bytes
                        i += 1;
                        j = spillOverSize / 4 - 1;  // Subtract 1 to offset the automatic increment in the loop
                    } else {
                        // Case 2: Record is not spanned, just read normally
                        // Copy the bytes to a Record object
                        memcpy(&r, this->getBlockAddress(i) + j, sizeof(Record));
                        // Print
                        cout << "Non-Span record: ";
                        r.printRecord();

                        // Skip to the next record
                        j += sizeof(Record) / 4 - 1;  // Subtract 1 to offset the automatic increment in the loop
                    }
                }
            }
        }
    }
}
