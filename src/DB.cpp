/**
 * @file DB.cpp
 * @brief Implementation of the DB class initialization method
 */

#include "../include/DB.h"
#include "../include/Logger.h"
#include "../include/Account.h"

#include <iostream>
#include <filesystem>

/**
 * @brief Initializes the database system
 * 
 * This method performs the following initialization steps:
 * 1. Checks if the data directory exists, creates it if it doesn't
 * 2. Creates Account.json file with empty array if it doesn't exist
 * 3. Creates Game.json file with empty array if it doesn't exist
 * 
 * All operations are logged using the Logger system.
 */
void DB::init() {
    LOG_INFO("Database checking...");
    // Check and create data directory
    if (!filesystem::exists(DATADIR)) {
        try {
            LOG_INFO("Data directory creating...");
            filesystem::create_directories(DATADIR);
            LOG_INFO("Data directory created!");
        } catch (const filesystem::filesystem_error& e) {
            LOG_ERROR("Error creating data directory: " + string(e.what()));
        }
    }
    else {
        LOG_INFO("Data directory exists!");
    }

    // Initialize Account.json if it doesn't exist
    if (!filesystem::exists(ACCOUNTDATA)) {
        try {
            ofstream file(ACCOUNTDATA);
            file << "[]";
            file.close();
        } catch (const exception& e) {
            LOG_ERROR("Error creating Account.json: " + string(e.what()));
        }
    }

    // Initialize Game.json if it doesn't exist
    if (!filesystem::exists(GAMEDATA)) {
        try {
            ofstream file(GAMEDATA);
            file << "[]";
            file.close();
        } catch (const exception& e) {
            LOG_ERROR("Error creating Game.json: " + string(e.what()));
        }
    }
}