#include "../include/Util.h"

void Util::waitEnter() {
    cout << endl << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    system("cls");
}

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