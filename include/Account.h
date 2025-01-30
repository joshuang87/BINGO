#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

class Account {
    private:
        string username;
        string password;

    public:
        Account() : username(""), password("") {}
        Account(string user, string pwd);

        // Getters
        string getUsername() const;
        string getPassword() const;

        // Setters
        void setUsername(string user);
        void setPassword(string pwd);

        static bool create(const Account& acc);
        static bool check(const Account& acc);

        // JSON parsing
        static vector<Account> from_json(const string& json);
        string to_json() const;

        ~Account() = default;
};

#endif // ACCOUNT_H