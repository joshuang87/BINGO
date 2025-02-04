/**
 * @file Util.cpp
 * @brief Implementation of utility functions used across the BINGO game
 */

#include "../include/Util.h"

/**
 * @brief Waits for user to press Enter and clears screen
 * 
 * This function:
 * 1. Displays a prompt for the user to press Enter
 * 2. Waits for user input
 * 3. Clears the screen after input is received
 */
void Util::waitEnter() {
    cout << endl << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    system("cls");
}

/**
 * @brief Displays a horizontal line for visual separation
 * 
 * Outputs a line of dashes to the console to create visual
 * separation between different sections of output.
 */
void Util::showLine() {
    cout << "--------------------------------------------------------------" << endl;
}

/**
 * @brief Checks if a given string represents a valid number.
 * 
 * This function iterates through each character of the input string to verify if it is a digit or a decimal point.
 * If any character is not a digit or a decimal point, the function returns false.
 * 
 * @param input The string to be checked.
 * @return true If the string represents a valid number.
 * @return false If the string does not represent a valid number.
 */
bool Util::isNumber(string input) {
    for (char c : input) {
        if (!isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}