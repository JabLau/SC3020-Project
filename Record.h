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
    // Data
	char movieId[11];
	float avgRating;
	unsigned int numVotes;


	Record();
    Record(string tconst, string averageRating, string numVotes);
    ~Record();
	
	void printRecord();

    void setNextSpanAddress(int *nextSpanPtr, int nextSpanLen);
};

