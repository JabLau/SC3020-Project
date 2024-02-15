#include "DiskManager.h"
#include <cstdlib>
#include "record.h"
#include <iostream>
#include <cstring>

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
	free(this->memStartAddress);
}

// Method to calculate block starting address by block id
int* DiskManager::getBlockAddress(int blockId) {
        return this->memStartAddress + (blockId * this->blockSize);
    }

// Method to get next available memory address
addressInfo DiskManager::getNextAvailableAddress() {
    // Loop through freeSpacePerBlock array to find the first block with at least 1 byte of free space
    for (int i = 0; i < this->totalNumBlocks; i++) {
            if (this->freeSpacePerBlock[i] >= 1) {
                // Iterate through block, finding the address of the first byte that is -1
                for (int j = 0; j < this->blockSize; j++) {
                    if (*(this->getBlockAddress(i) + j) == -1) {
                        // Return address struct addressinfo
                        return {i, j};

                    }
                }
            }
        }
    // If no available address, return nullptr
    return {-1, -1};
}

int* DiskManager::storeRecord(Record r) {
	// Check if any memory available
	if (totalRecords >= maxRecords ) {
		return nullptr;
	}

    // Find the next available address
    addressInfo nextAvailableAddress = getNextAvailableAddress();
    // Print next available address
    //cout << "Found empty slot at: " << nextAvailableAddress.blockId << ", " << nextAvailableAddress.offset << endl;

    // Case 1: Address is -1 -1, no available space
    if (nextAvailableAddress.blockId == -1 && nextAvailableAddress.offset == -1) {
        // Print out error message and raise exception
        cout << "No available space" << endl;
        return nullptr;
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
        int sizeLeft = freeSpacePerBlock[nextAvailableAddress.blockId];
        // Calculate the spillOverSize needed to store the record for next block
        int spillOverSize = sizeof(Record) - sizeLeft;

        // Copy first half of the record to the original block
        memcpy(getBlockAddress(nextAvailableAddress.blockId) + nextAvailableAddress.offset, &r, sizeLeft);
        // Update array to keep track of free space in each block in bytes
        this->freeSpacePerBlock[nextAvailableAddress.blockId] -= sizeLeft;

        // Copy spillOverSize record to the next available block
        // Since assuming fixed sized blocks, the next free space will always be the next block
        memcpy(getBlockAddress(nextAvailableAddress.blockId+1), &r, spillOverSize);
        // Update array to keep track of free space in each block in bytes
        this->freeSpacePerBlock[nextAvailableAddress.blockId+1] -= spillOverSize;
    }

    // Print relative address and return the absolute address of the record
    //cout << "Record inserted on block id, offset: " << nextAvailableAddress.blockId << ", " << nextAvailableAddress.offset << endl;
    return getBlockAddress(nextAvailableAddress.blockId) + nextAvailableAddress.offset;
}

// Sequentially read the records from the memory
void DiskManager::printAllRecords() {
    // Loop through blocks that have at least 1 record
    for (int i = 0; i < this->totalNumBlocks; i++) {
        // If block has at least 1 record, ignoring spillover records (Read from original block only)
        if (this->freeSpacePerBlock[i] <= this->blockSize - sizeof(Record)) {
            // Loop through the block
            for (int j = 0; j < this->blockSize; j++) {
                // print j
                // If the byte is not -1, it is the start of a record
                if (*(this->getBlockAddress(i) + j) != -1) {
                    // Read record
                    Record r;
                    // Case 1: Record is not spanned, just read normally
                    // Read the record and print
                    // Copy the bytes to a Record object
                    memcpy(&r, this->getBlockAddress(i) + j, sizeof(Record));
                    // Print
                    r.printRecord();

                    // Skip to the next record
                    j += sizeof(Record)/4 - 1;  // Subtract 1 to offset the automatic increment in the loop
                }
            }
        }
    }
}
