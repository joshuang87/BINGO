#ifndef PLAYER_H
#define PLAYER_H

#include "../include/Account.h"
#include <vector>
#include <string>

class Player : public Account {
    private:
        vector<vector<int>> board;
        vector<vector<bool>> marked;
        int gameCount;
        int winCount;
        int loseCount;
        double winRate;
        
    public:
        // CONSTRUCTOR
        Player(string user, string pwd);

        // GETTER
        int getGameCount() const;
        int getWinCount() const;
        int getLoseCount() const;
        double getWinRate() const;
        string getBoardState() const;  // Returns board state as JSON string

        // SETTER
        void setBoard(const vector<vector<int>>& board, const vector<vector<bool>>& marked);

        // DATA PROCESSING
        string to_json() const;
        static vector<Player> from_json(const string& json);

        // OTHER
        static Player* authenticator();
        static bool create(const Player& p);
        static bool check(const Player& p);
        void generateBoard();
        void displayBoard() const;
        bool markNumber(int num);
        bool checkWin();
        void updateStats(bool won);
};

#endif // PLAYER_H