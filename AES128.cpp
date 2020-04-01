#include <iostream>
#include "AES128.hpp"

AES128::AES128() {
    key_schedule = new unsigned char[KEY_SIZE_IN_BYTES];
}

void AES128::createKeySchedule(unsigned char* key) {
    this->key_schedule = getKeySchedule(key);
}

unsigned char* AES128::getKeySchedule(const unsigned char* key) {
    const int NUMBER_OF_ROUNDS = 11;
    
    unsigned char* key_schedule = new unsigned char[NUMBER_OF_ROUNDS * KEY_SIZE_IN_BYTES];
    
    for(int byte = 0; byte < KEY_SIZE_IN_BYTES; ++byte) {
        key_schedule[byte] = key[byte];
    }//Get first round
    
    for (int round = 1; round < NUMBER_OF_ROUNDS; ++round) {
        int offset = (round * KEY_SIZE_IN_BYTES);
        unsigned char* pointer_to_first_four_bytes_of_round = key_schedule + offset - 4;
        unsigned char* first_four_bytes_for_round_key = getFirstFourBytesForRoundKey(pointer_to_first_four_bytes_of_round, round);
        
        for (int byte = 0; byte < KEY_SIZE_IN_BYTES; ++byte) {
            bool the_first_four_bytes_are_being_operated_on = (byte < 4);
            int key_schedule_index = offset + byte;
            
            key_schedule[key_schedule_index] = key_schedule[key_schedule_index - KEY_SIZE_IN_BYTES];
            
            if (the_first_four_bytes_are_being_operated_on) {
                key_schedule[key_schedule_index] ^= first_four_bytes_for_round_key[byte];
            }else {
                unsigned char previous_word = key_schedule[key_schedule_index - 4];
                
                key_schedule[key_schedule_index] ^= previous_word;
            }
        }
        
        delete [] first_four_bytes_for_round_key;
    }
    
    return key_schedule;
}

unsigned char* AES128::getFirstFourBytesForRoundKey(const unsigned char* input, int round) {
    unsigned char* first_four_bytes_of_round_key = new unsigned char[4];
    const unsigned char FIRST_BYTE = input[0];
    
    //Shifting values
    first_four_bytes_of_round_key[0] = input[1];
    first_four_bytes_of_round_key[1] = input[2];
    first_four_bytes_of_round_key[2] = input[3];
    first_four_bytes_of_round_key[3] = FIRST_BYTE;
    
    //S-Box Mapping
    first_four_bytes_of_round_key[0] = sbox[first_four_bytes_of_round_key[0]] ^ rcon[round];
    first_four_bytes_of_round_key[1] = sbox[first_four_bytes_of_round_key[1]];
    first_four_bytes_of_round_key[2] = sbox[first_four_bytes_of_round_key[2]];
    first_four_bytes_of_round_key[3] = sbox[first_four_bytes_of_round_key[3]];
    
    return first_four_bytes_of_round_key;
}

void AES128::encrypt(unsigned char* input) {
    const int NUMBER_OF_ROUNDS = 10;
    performKeyAddition(input, key_schedule);
    
    for (int round = 1; round < NUMBER_OF_ROUNDS; ++round) {
        performByteSubstitution(input);
        shiftRows(input);
        mixColumns(input);
        performKeyAddition(input, (key_schedule + (round * KEY_SIZE_IN_BYTES)));
    }
    
    performByteSubstitution(input);
    shiftRows(input);
    performKeyAddition(input, (key_schedule + (NUMBER_OF_ROUNDS * KEY_SIZE_IN_BYTES)));
}

void AES128::performKeyAddition(unsigned char* input, const unsigned char* key) {
    for (int byte = 0; byte < KEY_SIZE_IN_BYTES; ++byte) {
        input[byte] ^= key[byte];
    }
}

void AES128::performByteSubstitution(unsigned char* input) {
    for (int byte = 0; byte < KEY_SIZE_IN_BYTES; ++byte) {
        input[byte] = sbox[input[byte]];
    }
}

void AES128::shiftRows(unsigned char* input) {
    unsigned char temp;
    
    temp      = input[1];
    input[1]  = input[5];
    input[5]  = input[9];
    input[9]  = input[13];
    input[13] = temp;
    
    input[2]  ^= input[10];
    input[10] ^= input[2];
    input[2]  ^= input[10];
    input[6]  ^= input[14];
    input[14] ^= input[6];
    input[6]  ^= input[14];

    temp      = input[3];
    input[3]  = input[15];
    input[15] = input[11];
    input[11] = input[7];
    input[7]  = temp;
}

void AES128::mixColumns(unsigned char *input) {
    unsigned char tmp[16];
    int i;
    for (i = 0; i < 4; ++i) {
        tmp[(i << 2) + 0] = (unsigned char) (mul2[input[(i << 2) + 0]] ^ mul_3[input[(i << 2) + 1]] ^ input[(i << 2) + 2] ^ input[(i << 2) + 3]);
        tmp[(i << 2) + 1] = (unsigned char) (input[(i << 2) + 0] ^ mul2[input[(i << 2) + 1]] ^ mul_3[input[(i << 2) + 2]] ^ input[(i << 2) + 3]);
        tmp[(i << 2) + 2] = (unsigned char) (input[(i << 2) + 0] ^ input[(i << 2) + 1] ^ mul2[input[(i << 2) + 2]] ^ mul_3[input[(i << 2) + 3]]);
        tmp[(i << 2) + 3] = (unsigned char) (mul_3[input[(i << 2) + 0]] ^ input[(i << 2) + 1] ^ input[(i << 2) + 2] ^ mul2[input[(i << 2) + 3]]);
    }

    for (i = 0; i < 16; ++i)
        input[i] = tmp[i];
}
