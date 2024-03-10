#include "Record.h"
#include <iostream>
#include <cstring>


Record::Record(string tconst, string averageRating, string numVotes) {
    // Load data from file
    // Convert string to char array
    strcpy(this->movieId, tconst.c_str()); // 11 bytes, 10 chars + null terminator
    // convert string to float
    this->avgRating = stof(averageRating); // 4 bytes
    // convert string to int
    this->numVotes = stoi(numVotes); // 4 bytes
}

Record::Record() {
}

Record::~Record() {

}

void Record::printRecord() {
    // Print record one line
    std::cout << "Movie ID: " << this->movieId << ", Average Rating: " << this->avgRating << ", Number of Votes: " << this->numVotes << std::endl;
}