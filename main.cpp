#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

// Record class
class Record {
public:
    std::string tconst;
    float averageRating;
    int numVotes;

    // Constructor
    Record(std::string tconst, std::string averageRating, std::string numVotes) {
        // Expected all arguments to be str type
        this->tconst = tconst;
        // convert string to float
        this->averageRating = std::stof(averageRating);
        //this->averageRating = averageRating;
        // convert string to int
        this->numVotes = std::stoi(numVotes);
        //this->numVotes = numVotes;
    }
};

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
        // First line is header
        if (line_num == 0) {
            std::cout << "Header: " << line << std::endl;
            line_num++;
            continue;
        }

        // Create record object
        std::vector<std::string> col = strSplitByDelim (line, '\t');
        Record record(col[0], col[1], col[2]);

        // Print the line
        std::cout << col[2] << std::endl;

        // Increment line number
        line_num++;

        // Break after 10 lines
        if (line_num > 10) {
            break;
        }
    }

    return 0;
}


