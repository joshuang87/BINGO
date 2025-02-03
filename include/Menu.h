#ifndef MENU_H
#define MENU_H

#include "Player.h"
#include "Leaderboard.h"
// #include "Game.h"

#include <vector>
using namespace std;

class Menu {
private:
    bool isRunning;
    vector<Player> players;
    Leaderboard leaderboard;
//     Game game;

    void displayCurrentTime();

public:
    Menu();
    void displayRules();
    void handleStartGame(Player& p1, Player& p2);
    void handleLoadGame(Player& p1, Player& p2);
    void handleSearchRecord();
    void handleViewLeaderboard();
    void exitProgram();
    void displayMainMenu(Player& p1, Player& p2);
};

#endif // MENU_H