/**
 * @file Account.cpp
 * @brief Implementation of the Account class methods
 */

#include "../include/Account.h"
#include "../include/DB.h"

#include <fstream>
#include <sstream>

/**
 * @brief Constructs an Account object with given username and password
 * @param user The username for the account
 * @param pwd The password for the account
 */
Account::Account(string user, string pwd) {
    username = user;
    password = pwd;
}

/**
 * @brief Gets the username of the account
 * @return The username as a string
 */
string Account::getUsername() const {
    return username;
}

/**
 * @brief Gets the password of the account
 * @return The password as a string
 */
string Account::getPassword() const {
    return password;
}

/**
 * @brief Sets the username of the account
 * @param user The new username to set
 */
void Account::setUsername(string user) {
    this->username = user;
}

/**
 * @brief Sets the password of the account
 * @param pwd The new password to set
 */
void Account::setPassword(string pwd) {
    this->password = pwd;
}

/**
 * @brief Creates a new account in the database
 * @param acc The account to create
 * @return true if account creation was successful, false if account already exists
 */
bool Account::create(const Account& acc) {
    if (check(acc)) return false;
    return DB::getInstance().save(acc);
}

/**
 * @brief Checks if an account exists in the database
 * @param acc The account to check
 * @return true if account exists, false otherwise
 * 
 * This method loads all accounts from the database and checks if there's
 * a matching username and password combination.
 */
bool Account::check(const Account& acc) {
    vector<Account> accList = DB::getInstance().load<Account>();
    if (accList.empty()) {
        return false;
    }

    for (Account x : accList) {
        if (acc.getUsername() == x.getUsername() && acc.getPassword() == x.getPassword()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Parses a JSON string into a vector of Account objects
 * @param json The JSON string to parse
 * @return Vector of Account objects
 * 
 * This method parses a JSON string containing account information and
 * creates Account objects from it. The JSON format should be:
 * {"username":"value","password":"value"}
 */
vector<Account> Account::from_json(const string& json) {
    vector<Account> accounts;
    stringstream ss(json);
    string line;

    while (getline(ss, line, '{')) {
        // Skip empty entries
        if (line.find("username") == string::npos) continue;

        Account acc;
        size_t pos;

        // Parse username
        pos = line.find("username");
        string username = line.substr(pos + 11); // skip "username":"
        acc.setUsername(username.substr(0, username.find("\"")));

        // Parse password
        pos = line.find("password");
        string password = line.substr(pos + 11); // skip "password":"
        acc.setPassword(password.substr(0, password.find("\"")));

        accounts.push_back(acc);
    }

    return accounts;
}

/**
 * @brief Converts the account to a JSON string
 * @return JSON string representation of the account in the format:
 *         {"username":"value","password":"value"}
 */
string Account::to_json() const {
    return "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";
}