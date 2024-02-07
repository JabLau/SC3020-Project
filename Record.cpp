#include "Record.h"
#include <iostream>
#include <cstring>

Record::Record() {

}

Record::Record(string tconst, float avgRating, unsigned int numVotes) {
	strcpy(this->movieId, tconst.c_str());
	this->avgRating = avgRating;
	this->numVotes = numVotes;
}

Record::Record(string tconst, string averageRating, string numVotes) {
    // Convert string to char array
    strcpy(this->movieId, tconst.c_str()); // 11 bytes, 10 chars + null terminator
    // convert string to float
    this->avgRating = stof(averageRating); // 4 bytes
    // convert string to int
    this->numVotes = stoi(numVotes); // 4 bytes
}

void Record::setNextSpanAddress(int* nextSpanAddr, int nextSpanLen) {
    this->nextSpanPtr = nextSpanAddr;
    this->nextSpanLen = nextSpanLen;
}

//void Record::setRecordAddress(int blockNum, int offset) {
//    this->blockNum = blockNum;
//    this->offset = offset;
//}

int Record::minimumSpace(){
    return (sizeof(this->nextSpanPtr) + sizeof(this->nextSpanLen));
}

Record::~Record() {

}

void Record::printRecord() {
	cout << "tconst:" << this->movieId << "|avgRating:" << this->avgRating << "|numVotes:" << this->numVotes << endl;
}