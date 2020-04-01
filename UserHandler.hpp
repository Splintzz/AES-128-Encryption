#ifndef UserHandler_hpp
#define UserHandler_hpp

#pragma once
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class UserHandler {
    public:
        UserHandler();
        char* getFilePath(string);
        unsigned char* getKey();
    private:
        void flushInput();
        string getFixedKey(string);
        string removeWhiteSpace(string);
        string applyCorrectSpacing(string);
};

#endif /* UserHandler_hpp */
