//
// Created by Aozy on 7/2/2024.
//

#ifndef SC3020_PROJECT_FILEMANAGER_H
#define SC3020_PROJECT_FILEMANAGER_H
#include <string>
#include <vector>
#include "DiskManager.h"

using namespace std;

class FileManager {
public:
    // counter for number of records stored successfully
    int recordCount = 0;
    int blockCount = 0;

    vector<string> strSplitByDelim (const string &s, char delim);
    void load_data(DiskManager &disk);
};


#endif //SC3020_PROJECT_FILEMANAGER_H
