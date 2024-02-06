#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdint>

// Str split function
std::vector<std::string> strSplitByDelim (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

// Record class
// the disk capacity could be 100 - 500 MB
class Record {
public:
    // Atrtibutes
    char movieId[11];
    //std::string movieId; //alphanumeric unique identifier of the title
    float avgRating; //weighted average of all the individual user ratings
    int numVotes; // number of votes the title has received

    // Constructor
    // Expected all arguments to be str type
    Record(std::string tconst, std::string averageRating, std::string numVotes) {
        // Convert string to char array
        strcpy(this->movieId, tconst.c_str()); // 11 bytes, 10 chars + null terminator
        // convert string to float
        this->avgRating = std::stof(averageRating); // 4 bytes
        // convert string to int
        this->numVotes = std::stoi(numVotes); // 4 bytes
    }
};


int main() {
    // Load data.tsv file into
    std::ifstream infile("../data.tsv");

    // Line-based parsing, using string streams
    // temp variable to store the line
    std::string line;
    // temp var for line num
    unsigned int line_num = 0;
    // Read the file line by line
    while ( std::getline(infile, line) )
    {
        // Process each line
        // Delimiter is tab
        // First line is header, skip it
        if (line_num == 0) {
            std::cout << "Header: " << line << std::endl;
            line_num++;
            continue;
        }

        // For every line, create a record object
        // Split line by delim into array of strings
        std::vector<std::string> col = strSplitByDelim (line, '\t');
        // Create record object
        Record record(col[0], col[1], col[2]);

        // Increment line number
        line_num++;

        // Break after 10 lines
        if (line_num > 10) {
            break;
        }
    }

    return 0;
}


