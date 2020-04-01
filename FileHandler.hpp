#ifndef FileHandler_hpp
#define FileHandler_hpp

#pragma once
#include <stdio.h>
#include <string>

using namespace std;

class FileHandler {
private:
    FILE* read_file;
    FILE* write_file;
    char* read_file_path;
    char* write_file_path;
    int bytes_read_from_last_buffer_fill;
    
public:
    FileHandler();
    void setReadFilePath(char*);
    void setWriteFilePath(char*);
    void configureWriteFile();
    void openFiles();
    void checkForErrors();
    void padBuffer(unsigned char*, int);
    unsigned char* getFullBufferPadding();
    void fillBuffer(unsigned char*);
    int getBytesReadFromLastBufferFill();
    void writeEncyptionToFile(unsigned char*);
    int getSupportedBufferSize();
    void closeFiles();
};

static const int SIZE_OF_BUFFER = 16;

#endif /* FileHandler_hpp */
