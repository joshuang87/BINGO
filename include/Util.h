#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

class Util {
public:
    // Make all methods static since they are utility functions
    static void waitEnter();
    static void showLine();
    static bool isNumber(string input);
};

#endif // UTIL_H
