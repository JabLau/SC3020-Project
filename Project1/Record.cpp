#include "Record.h"
#include <iostream>

Record::Record() {

}

Record::Record(string tconst, float avgRating, unsigned int numVotes) {
	strcpy_s(this->movieId, tconst.c_str());
	this->avgRating = avgRating;
	this->numVotes = numVotes;
}

Record::~Record() {

}

void Record::printRecord() {
	cout << "tconst:" << this->movieId << "|avgRating:" << this->avgRating << "|numVotes:" << this->numVotes << endl;
}