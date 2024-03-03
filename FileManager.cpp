#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "FileManager.h"
#include "Record.h"
#include "DiskManager.h"
#include "tempStruct.h"
#include <iostream>

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

vector<tempStruct> FileManager::load_data(DiskManager* disk) {
    // Load data.tsv file into
    ifstream infile("../data.tsv");
    //define array here.
    std::vector<tempStruct> addr_NumVotes_list;
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

        // Store record in disk
        addressInfo addr = disk->storeRecord(Record(col[0], col[1], col[2]));
        // If record is stored successfully, increment totalRecords
        tempStruct s1 = {stoi(col[2]), disk->getBlockAddress(addr.blockId)+addr.offset};
        addr_NumVotes_list.push_back(s1);
        if (addr.blockId != -1 && addr.offset != -1){
            recordCount++;
        }

        // Store the highest recordId
        if (addr.blockId+1 > blockCount) {
            blockCount = addr.blockId+1;
        }

        // Increment line number
        line_num++;
        cout << line_num << endl;

//        // Break after 10 lines
//        if (line_num > 10) {
//            break;
//        }
    }
    return addr_NumVotes_list;
}


