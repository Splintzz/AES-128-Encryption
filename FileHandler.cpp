#include "FileHandler.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

FileHandler::FileHandler() {
    bytes_read_from_last_buffer_fill = 0;
}

void FileHandler::setReadFilePath(char* file_path) {
    read_file_path = file_path;
    read_file = fopen(read_file_path, "rb");
    
    bool there_was_an_error_with_the_read_file = !read_file;
    if (there_was_an_error_with_the_read_file) {
        cerr << "There was an error with the read in file." << endl;
        exit(-1);
    }
}

void FileHandler::setWriteFilePath(char* file_path) {
    write_file_path = file_path;
    write_file = fopen(write_file_path, "wb");
    
    bool there_was_an_error_with_the_write_file = !write_file;
    if (there_was_an_error_with_the_write_file) {
        cerr << "There was an error with the write out file." << endl;
        exit(-1);
    }
}

void FileHandler::configureWriteFile() {
    const string ENCODING_FILE_EXTENSION = ".enc";
    fstream file;
    string file_path(read_file_path);
    
    size_t file_extension_index = file_path.rfind('.');
    
    string write_file_name = file_path.substr(0, file_extension_index) + ENCODING_FILE_EXTENSION;
    file.open(write_file_name, ios::out);
    
    char* write_file_path = new char[file_path.length()];
    strcpy(write_file_path, write_file_name.c_str());
    
    setWriteFilePath(write_file_path);
}

void FileHandler::fillBuffer(unsigned char* bufffer_to_fill) {
    const size_t SIZE = sizeof(char);
    
    bytes_read_from_last_buffer_fill = fread(bufffer_to_fill, SIZE, SIZE_OF_BUFFER, read_file);
}

void FileHandler::writeEncyptionToFile(unsigned char* encryption_data) {
    const size_t SIZE = sizeof(char);
    
    fwrite(encryption_data, SIZE, SIZE_OF_BUFFER, write_file);
}

void FileHandler::padBuffer(unsigned char* buffer, int bytes_read) {
    unsigned int padding_value = SIZE_OF_BUFFER - bytes_read;
    
    for (int byte = bytes_read; byte < SIZE_OF_BUFFER; ++byte) {
        buffer[byte] = padding_value;
    }
}

unsigned char* FileHandler::getFullBufferPadding() {
    unsigned char* full_buffer_padding = new unsigned char[SIZE_OF_BUFFER];
    
    for (int byte = 0; byte < SIZE_OF_BUFFER; ++byte) {
        full_buffer_padding[byte] = SIZE_OF_BUFFER;
    }
    
    return full_buffer_padding;
}

int FileHandler::getBytesReadFromLastBufferFill() {
    return bytes_read_from_last_buffer_fill;
}

int FileHandler::getSupportedBufferSize() {
    return SIZE_OF_BUFFER;
}

void FileHandler::closeFiles() {
    fclose(read_file);
    fclose(write_file);
}
