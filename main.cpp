#include <iostream>
#include <sstream>
#include "AES128.hpp"
#include "FileHandler.hpp"
#include "UserHandler.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    FileHandler file_handler = FileHandler();
    UserHandler user_handler = UserHandler();
    AES128 encryptor = AES128();
    
    //Handle read and write files
    file_handler.setReadFilePath(user_handler.getFilePath("read"));
    file_handler.configureWriteFile();
    
    unsigned char* key = user_handler.getKey();
    encryptor.createKeySchedule(key);

    unsigned char* buffer = new unsigned char[file_handler.getSupportedBufferSize()];
    file_handler.fillBuffer(buffer);

    int bytes_read = file_handler.getBytesReadFromLastBufferFill();
    bool there_are_bytes_to_encrypt = (bytes_read > 0);

    //encryption algorithm
    while (there_are_bytes_to_encrypt) {

        if (bytes_read == file_handler.getSupportedBufferSize()) {
            encryptor.encrypt(buffer);
            encryptor.encrypt(file_handler.getFullBufferPadding());
        } else {
            file_handler.padBuffer(buffer, bytes_read);
            encryptor.encrypt(buffer);
        }

        file_handler.writeEncyptionToFile(buffer);
        file_handler.fillBuffer(buffer);

        bytes_read = file_handler.getBytesReadFromLastBufferFill();
        there_are_bytes_to_encrypt = (bytes_read > 0);
    }

    delete [] key;
    delete [] buffer;
    
    file_handler.closeFiles();

    return 0;
}
