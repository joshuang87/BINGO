#ifndef GAME_H
#define GAME_H

#include "Player.h"

#include <vector>
#include <string>
#include <set>

using namespace std;

class Game {
    private:
        vector<Player> players;
        Player* winner;
        int currentTurn;
        bool isOver;
        bool isSaved = false;
        set<int> usedNumbers;
        string gameId;

    public:
        // CONSTRUCTOR
        Game(bool empty = false);

        // GETTER
        const vector<Player>& getPlayers() const;
        Player* getWinner() const;
        string getGameId() const;
        string getCurrentTurn() const;

        //SETTER
        void setWinner(Player* player);
        void setGameId(const string& gameId);

        // VALIDATION
        bool isGameIdExist(const string& gameId);
        bool isGameOver() const;
        bool isGameSaved() const;

        // DATA PROCESSING
        static vector<Game> from_json(const string& json);
        void save(vector<Game>& games);
        void save();
        static vector<vector<int>> parseGameStatusData(const string& json);
        static vector<vector<bool>> parseGameMarkedData(const vector<vector<int>>& data);

        // OTHER
        // bool loadGameState(const string& roomId);s
        void cleanupRoom();
        static bool displaySavedGames(vector<Game>& games, Player& p1, Player& p2);
        void startGame(vector<Player*> players);
        void playTurn();
        string generateGameId();
        void continueGame();
};

#endif