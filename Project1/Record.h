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
	char movieId[11];
	float avgRating;
	unsigned int numVotes;

	Record();
	Record(string tconst, float avgRating, unsigned int numVotes);
	~Record();
	
	void printRecord();
};

