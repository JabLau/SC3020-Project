#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdint>
#include "FileManager.h"
#include "Record.h"
#include "DiskManager.h"

using namespace std;

// Str split function
vector<string> FileManager::strSplitByDelim(const string &s, char delim) {
    vector<std::string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


void FileManager::load_data(DiskManager &disk) {
    // Load data.tsv file into
    ifstream infile("../data.tsv");

    // Line-based parsing, using string streams
    // temp variable to store the line
    string line;
    // temp var for line num
    unsigned int line_num = 0;
    // Read the file line by line
    while ( getline(infile, line) )
    {
        // Process each line
        // Delimiter is tab
        // First line is header, skip it
        if (line_num == 0) {
            line_num++;
            continue;
        }

        // Split line by delim into array of strings
        vector<string> col = strSplitByDelim (line, '\t');
        // For every line, create a record object
        // Store record in disk
        disk.storeRecord(Record(col[0], col[1], col[2]));

        // Increment line number
        line_num++;

        // Break after 10 lines
        if (line_num > 10) {
            break;
        }
    }
}


