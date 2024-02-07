#pragma once
#define Record_H
#include <string>

// Stores Record Data
// tconst	| averageRating	| numVotes
// string	| Float			| Unsigned int     

using namespace std;

class Record
{
public:
    // Header
    // Span
    int* nextSpanPtr;
    int nextSpanLen;


    int offset;
    int* nextRecPtr;
    // Data
	char movieId[11];
	float avgRating;
	unsigned int numVotes;


	Record();
	Record(string tconst, float avgRating, unsigned int numVotes);
    Record(string tconst, string averageRating, string numVotes);
    ~Record();
	
	void printRecord();
    void setRecordAddress(int blockNum, int offset);
    int minimumSpace();
};

