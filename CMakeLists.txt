cmake_minimum_required(VERSION 3.27)
project(SC3020_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SC3020_Project
        BPTree.cpp
        BPTree.h
        DiskManager.cpp
        DiskManager.h
        FileManager.cpp
        FileManager.h
        List.cpp
        List.h
        main.cpp
        Node.cpp
        Node.h
        Record.cpp
        Record.h
        tempStruct.cpp
        tempStruct.h)
