/**
 * @file Util.h
 * @brief Header file for utility functions used across the BINGO game
 */

#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

/**
 * @class Util
 * @brief Static utility class providing common functionality
 * 
 * This class contains utility methods that are used across different
 * parts of the application. All methods are static as they don't
 * require instance-specific data.
 */
class Util {
public:
    /**
     * @brief Waits for user to press Enter and clears screen
     */
    static void waitEnter();

    /**
     * @brief Displays a horizontal line for visual separation
     */
    static void showLine();

    /**
     * @brief Checks if a string represents a valid number
     * @param input The string to check
     * @return true if string is a valid number, false otherwise
     */
    static bool isNumber(string input);
};

#endif // UTIL_H
