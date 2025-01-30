#include "../include/DB.h"
#include "../include/Logger.h"
#include "../include/Account.h"

#include <iostream>
#include <filesystem>

void DB::init() {
    LOG_INFO("Database checking...");
    // 检查并创建数据目录
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

    if (!filesystem::exists(ACCOUNTDATA)) {
        try {
            ofstream file(ACCOUNTDATA);
            file << "[]";
            file.close();
        } catch (const exception& e) {
            
        }
    }

    if (!filesystem::exists(GAMEDATA)) {
        try {
            ofstream file(GAMEDATA);
            file << "[]";
            file.close();
        } catch (const exception& e) {
            
        }
    }
}

template bool DB::save<Account>(const Account& acc);

template vector<Account> DB::load<Account>();