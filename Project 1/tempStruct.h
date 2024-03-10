//
// Created by Ivan on 13/2/2024.
//

#ifndef SC3020_PROJECT_TEMPSTRUCT_H
#define SC3020_PROJECT_TEMPSTRUCT_H

#include <vector>

using namespace std;

struct tempStruct{
    int key;
    int* address;
};

struct tempStruct2{
    int key;
    vector<int*> addresses;
};

#endif //SC3020_PROJECT_TEMPSTRUCT_H
