/**
 * @file Menu.h
 * @brief Header file for the Menu class that manages the game's user interface
 */

#ifndef MENU_H
#define MENU_H

#include "Player.h"
#include "Leaderboard.h"
// #include "Game.h"

#include <vector>
using namespace std;

/**
 * @class Menu
 * @brief Manages the game's main menu and user interface
 * 
 * This class handles all menu-related operations including displaying
 * the main menu, game rules, and managing different game options like
 * starting a new game, loading saved games, and viewing statistics.
 */
class Menu {
private:
    bool isRunning;           ///< Flag to control the main menu loop
    vector<Player> players;   ///< Collection of players
    Leaderboard leaderboard; ///< Leaderboard instance for displaying rankings
//     Game game;

    /**
     * @brief Displays the current system time
     */
    void displayCurrentTime();

public:
    /**
     * @brief Constructor for Menu class
     * 
     * Initializes the menu with isRunning set to true.
     */
    Menu();

    /**
     * @brief Displays the game rules to the player
     */
    void displayRules();

    /**
     * @brief Handles starting a new game
     * @param p1 Reference to Player 1
     * @param p2 Reference to Player 2
     */
    void handleStartGame(Player& p1, Player& p2);

    /**
     * @brief Handles loading a saved game
     * @param p1 Reference to Player 1
     * @param p2 Reference to Player 2
     */
    void handleLoadGame(Player& p1, Player& p2);

    /**
     * @brief Handles searching and displaying player records
     */
    void handleSearchRecord();

    /**
     * @brief Handles displaying the leaderboard
     */
    void handleViewLeaderboard();

    /**
     * @brief Handles program exit
     * 
     * Sets isRunning to false and displays goodbye message.
     */
    void exitProgram();

    /**
     * @brief Displays and handles the main menu
     * @param p1 Reference to Player 1
     * @param p2 Reference to Player 2
     * 
     * Shows menu options and processes user input in a loop until
     * the program is exited.
     */
    void displayMainMenu(Player& p1, Player& p2);
};

#endif // MENU_H