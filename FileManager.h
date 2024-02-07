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
    vector<string> strSplitByDelim (const string &s, char delim);
    bool load_data(DiskManager &disk);
};


#endif //SC3020_PROJECT_FILEMANAGER_H