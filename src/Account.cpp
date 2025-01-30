#include "../include/Account.h"
#include "../include/DB.h"

#include <fstream>
#include <sstream>

Account::Account(string user, string pwd) {
    username = user;
    password = pwd;
}

string Account::getUsername() const {
    return username;
}

string Account::getPassword() const {
    return password;
}

void Account::setUsername(string user) {
    this->username = user;
}

void Account::setPassword(string pwd) {
    this->password = pwd;
}

bool Account::create(const Account& acc) {
    if (check(acc)) return false;
    return DB::getInstance().save(acc);
}

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

string Account::to_json() const {
    return "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";
}