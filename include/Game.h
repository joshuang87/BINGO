/**
 * @file Game.h
 * @brief Header file for the Game class that manages BINGO game logic
 */

#ifndef GAME_H
#define GAME_H

#include "Player.h"

#include <vector>
#include <string>
#include <set>

using namespace std;

/**
 * @class Game
 * @brief Manages the game logic and state for a BINGO game
 * 
 * This class handles all aspects of a BINGO game including:
 * - Player management
 * - Turn management
 * - Game state persistence
 * - Win condition checking
 */
class Game {
    private:
        vector<Player> players;      ///< List of players in the game
        Player* winner;              ///< Pointer to the winning player (nullptr if no winner)
        int currentTurn;            ///< Index of the current player's turn
        bool isOver;                ///< Flag indicating if the game is finished
        bool isSaved = false;       ///< Flag indicating if the game state is saved
        set<int> usedNumbers;       ///< Set of numbers that have been called
        string gameId;              ///< Unique identifier for the game

    public:
        /**
         * @brief Constructor for Game class
         * @param empty If true, creates an empty game without generating ID
         */
        Game(bool empty = false);

        /**
         * @brief Gets the list of players in the game
         * @return Constant reference to the vector of players
         */
        const vector<Player>& getPlayers() const;

        /**
         * @brief Gets the winner of the game
         * @return Pointer to winning player, nullptr if no winner
         */
        Player* getWinner() const;

        /**
         * @brief Gets the game's unique identifier
         * @return String containing the game ID
         */
        string getGameId() const;

        /**
         * @brief Gets the current turn number
         * @return String representation of current turn
         */
        string getCurrentTurn() const;
        
        /**
         * @brief Sets the winner of the game
         * @param player Pointer to the winning player
         */
        void setWinner(Player* player);

        /**
         * @brief Sets the game's unique identifier
         * @param gameId String containing the new game ID
         */
        void setGameId(const string& gameId);

        /**
         * @brief Checks if a game ID already exists
         * @param gameId The game ID to check
         * @return true if game ID exists, false otherwise
         */
        bool isGameIdExist(const string& gameId);

        /**
         * @brief Checks if the game is over
         * @return true if game is over, false otherwise
         */
        bool isGameOver() const;

        /**
         * @brief Checks if the game state is saved
         * @return true if game is saved, false otherwise
         */
        bool isGameSaved() const;

        /**
         * @brief Parses a JSON string into a vector of Game objects
         * @param json The JSON string to parse
         * @return Vector of Game objects
         */
        static vector<Game> from_json(const string& json);

        /**
         * @brief Saves multiple games to storage
         * @param games Vector of games to save
         */
        void save(vector<Game>& games);

        /**
         * @brief Saves the current game state to storage
         */
        void save();

        /**
         * @brief Parses game status data from JSON
         * @param json The JSON string containing game status
         * @return Vector of vectors containing game board numbers
         */
        static vector<vector<int>> parseGameStatusData(const string& json);

        /**
         * @brief Converts game board numbers to marked status
         * @param data Vector of vectors containing game board numbers
         * @return Vector of vectors containing marked status (true/false)
         */
        static vector<vector<bool>> parseGameMarkedData(const vector<vector<int>>& data);

        /**
         * @brief Handles cleanup when exiting a game room
         * Prompts for saving if game is not over
         */
        void cleanupRoom();

        /**
         * @brief Displays list of saved games for given players
         * @param games Vector of all saved games
         * @param p1 First player
         * @param p2 Second player
         * @return true if saved games exist for players, false otherwise
         */
        static bool displaySavedGames(vector<Game>& games, Player& p1, Player& p2);

        /**
         * @brief Starts a new game with given players
         * @param players Vector of pointers to players
         */
        void startGame(vector<Player*> players);

        /**
         * @brief Handles the logic for a single turn in the game
         */
        void playTurn();

        /**
         * @brief Generates a unique game ID
         * @return String containing the generated game ID
         */
        string generateGameId();

        /**
         * @brief Continues a saved game
         */
        void continueGame();

        /**
         * @brief Loads player statistics from source players to current game players
         * @param ps Vector of source players containing statistics to be loaded
         * 
         * Updates the current game's players' statistics (game count, win count,
         * lose count, and win rate) from the provided source players.
         */
        void loadPlayerData(vector<Player>& ps);
};

#endif