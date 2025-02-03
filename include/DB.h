/**
 * @file DB.h
 * @brief Database management class header implementing singleton pattern
 * 
 * This file contains the DB class which handles all database operations
 * including saving, loading, and resetting data in JSON format.
 */

#ifndef DB_H
#define DB_H

#include "../include/Logger.h"
#include "../include/Account.h"
#include "../include/Game.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>
#include <stdexcept>

using namespace std;

/**
 * @class DB
 * @brief Singleton class for managing database operations
 * 
 * This class implements the singleton pattern and provides methods for
 * saving, loading, and resetting data in JSON format. It supports different
 * data types through template methods.
 */
class DB {
    public:
        /**
         * @brief Gets the singleton instance of the DB class
         * @return Reference to the singleton DB instance
         */
        static DB& getInstance() {
            static DB instance;
            return instance;
        }

        // Delete copy constructor and assignment operator
        DB(const DB&) = delete;
        DB& operator=(const DB&) = delete;

        /**
         * @brief Initializes the database by creating necessary directories and files
         */
        void init();

        /**
         * @brief Generic method to save data to JSON file
         * @tparam T The type of data to save
         * @param data The data object to save
         * @return true if save was successful, false otherwise
         * 
         * This method saves data in JSON format. If the file doesn't exist,
         * it creates a new file with an empty array. New data is appended
         * to the existing array.
         */
        template<typename T>
        bool save(const T& data) {
            string filename = getFilename<T>();
            string jsonData = data.to_json();
            
            try {
                // Check if file exists, if not create and initialize with empty array
                if (!filesystem::exists(filename)) {
                    ofstream newFile(filename);
                    newFile << "[]";
                    newFile.close();
                }

                // Read existing file content
                ifstream inFile(filename);
                string content;
                getline(inFile, content);
                inFile.close();

                // Handle file content
                if (content.empty()) {
                    content = "[]";
                }

                // Insert new data at the end
                if (content == "[]") {
                    // If empty array, directly insert data
                    content = "[" + jsonData + "]";
                } else {
                    // Insert new data before the last ']'
                    content.insert(content.length() - 1, "," + jsonData);
                }

                // Write updated content
                ofstream outFile(filename);
                if (!outFile.is_open()) {
                    LOG_ERROR("Failed to open file: " + filename);
                    return false;
                }
                
                outFile << content;
                outFile.close();
                LOG_INFO("Data saved to " + filename);
                return true;
            } catch (const exception& e) {
                LOG_ERROR("Error saving data: " + string(e.what()));
                return false;
            }
        }

        /**
         * @brief Generic method to load data from JSON file
         * @tparam T The type of data to load
         * @return Vector of objects of type T
         * 
         * This method loads and parses JSON data from a file into objects.
         * If the file doesn't exist or is empty, returns an empty vector.
         */
        template<typename T>
        vector<T> load() {
            string filename = getFilename<T>();
            vector<T> results;
            
            try {
                // Check if file exists
                if (!filesystem::exists(filename)) {
                    LOG_INFO("File not found: " + filename);
                    return results;
                }

                // Read file content
                ifstream inFile(filename);
                string content;
                getline(inFile, content);
                inFile.close();

                // Handle empty file or empty array
                if (content.empty() || content == "[]") {
                    return results;
                }

                // Parse JSON array
                results = T::from_json(content);

                LOG_INFO("Data loaded from " + filename);
                return results;
            } catch (const exception& e) {
                LOG_ERROR("Error loading data: " + string(e.what()));
                return results;
            }
        }

        /**
         * @brief Generic method to reset data file
         * @tparam T The type of data file to reset
         * @return true if reset was successful
         * 
         * This method resets the specified data file by creating
         * a new empty array.
         */
        template<typename T>
        bool reset() {
            string filename = getFilename<T>();
            ofstream newFile(filename, ios::trunc);
            newFile << "[]";
            newFile.close();

            return true;
        }

    private:
        /// Path to the data directory
        const string DATADIR = "../data";
        /// Path to the account data file
        const string ACCOUNTDATA = "../data/Account.json";
        /// Path to the game data file
        const string GAMEDATA = "../data/Game.json";

        /**
         * @brief Private constructor for singleton pattern
         */
        DB() {}

        /**
         * @brief Gets the filename for a specific data type
         * @tparam T The type of data
         * @return String containing the filename
         * @throw runtime_error if data type is not supported
         */
        template<typename T>
        string getFilename() const {
            if (typeid(T) == typeid(Account) || typeid(T) == typeid(Player)) {
                return ACCOUNTDATA;
            } 
            else if (typeid(T) == typeid(Game)) {
                return GAMEDATA;
            }
            throw runtime_error("Unsupported data type");
        }
};

#endif // DB_H