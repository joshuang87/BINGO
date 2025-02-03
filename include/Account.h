/**
 * @file Account.h
 * @brief Header file for the Account class which handles user authentication
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

/**
 * @class Account
 * @brief Represents a user account with username and password
 * 
 * This class handles user account operations including creation, authentication,
 * and JSON serialization/deserialization.
 */
class Account {
    private:
        string username;  ///< The username of the account
        string password;  ///< The password of the account

    public:
        /**
         * @brief Default constructor
         * Initializes an empty account with empty username and password
         */
        Account() : username(""), password("") {}
        
        /**
         * @brief Parameterized constructor
         * @param user The username for the account
         * @param pwd The password for the account
         */
        Account(string user, string pwd);

        /**
         * @brief Get the username of the account
         * @return The username as a string
         */
        string getUsername() const;

        /**
         * @brief Get the password of the account
         * @return The password as a string
         */
        string getPassword() const;

        /**
         * @brief Set the username of the account
         * @param user The new username to set
         */
        void setUsername(string user);

        /**
         * @brief Set the password of the account
         * @param pwd The new password to set
         */
        void setPassword(string pwd);

        /**
         * @brief Create a new account in the database
         * @param acc The account to create
         * @return true if account creation was successful, false if account already exists
         */
        static bool create(const Account& acc);

        /**
         * @brief Check if an account exists in the database
         * @param acc The account to check
         * @return true if account exists, false otherwise
         */
        static bool check(const Account& acc);

        /**
         * @brief Parse a JSON string into a vector of Account objects
         * @param json The JSON string to parse
         * @return Vector of Account objects
         */
        static vector<Account> from_json(const string& json);

        /**
         * @brief Convert the account to a JSON string
         * @return JSON string representation of the account
         */
        string to_json() const;

        /**
         * @brief Default destructor
         */
        ~Account() = default;
};

#endif // ACCOUNT_H