#include "UserHandler.hpp"
#include "AES128.hpp"
#include <string>

using namespace std;

UserHandler::UserHandler() {}

char* UserHandler::getFilePath(string read_or_write) {
    string file_path;
    
    cout << "Enter a the path of the file to " << read_or_write << " from: ";
    cin >> file_path;
    
    char* file_path_array = new char[file_path.length()];
    strcpy(file_path_array, file_path.c_str());
    
    return file_path_array;
}

unsigned char* UserHandler::getKey() {
    const int REQUIRED_SIZE_OF_KEY = 47; //32 digit hex number + 15 spaces in between every two hex numbers

    unsigned char* key = new unsigned char[KEY_SIZE_IN_BYTES];
    string key_input_by_user = "";
    bool input_was_invalid = true;
     
    flushInput();
    while (input_was_invalid) {
        cout << "Enter a 32 hexadecimal key: ";
        getline(cin, key_input_by_user);

        key_input_by_user = getFixedKey(key_input_by_user);
        input_was_invalid = (key_input_by_user.length() != REQUIRED_SIZE_OF_KEY);
    }
    
    istringstream key_stream(key_input_by_user);
    unsigned int character;
    
    for (int character_count = 0; (key_stream >> hex >> character); ++character_count) {
        key[character_count] = character;
    }
    
    return key;
}

void UserHandler::flushInput() {
    cin.clear();
    cin.ignore(INT_MAX, '\n');
}

string UserHandler::getFixedKey(string key) {
    key = removeWhiteSpace(key);
    key = applyCorrectSpacing(key);
    
    return key;
}

string UserHandler::removeWhiteSpace(string key) {
    string correctly_formated_key = "";
    
    for (int key_index = 0; key_index < key.length(); ++key_index) {
        char character_in_key = key[key_index];
        
        if (isalnum(character_in_key)) {
            correctly_formated_key += character_in_key;
        }
    }
    
    return correctly_formated_key;
}

string UserHandler::applyCorrectSpacing(string key) {
    bool the_key_does_not_meet_the_length_requirement = (key.length() != 32);
    
    if (the_key_does_not_meet_the_length_requirement) {
        return key;
    }
    //if the key is not 32 characters in length after removing the spacing,
    //return the key and let the original while loop handle it
    
    const char SPACE = ' ';
    const int SPACING_CONSTANT = 2;
    
    string correctly_spaced_key = "";
    correctly_spaced_key += key[0];
    
    for (int key_index = 1; key_index < key.length(); ++key_index) {
        if (key_index % SPACING_CONSTANT == 0) {
            correctly_spaced_key += SPACE;
        }
        
        correctly_spaced_key += key[key_index];
    }
    
    return correctly_spaced_key;
}
