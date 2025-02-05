/**
 * @file Player.h
 * @brief Header file for the Player class that represents a player in the BINGO game
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "../include/Account.h"
#include <vector>
#include <string>

using namespace std;

/**
 * @class Player
 * @brief Represents a player in the BINGO game, inheriting from Account
 * 
 * This class manages player-specific functionality including:
 * - BINGO board generation and manipulation
 * - Game statistics tracking
 * - Player authentication and creation
 * - JSON serialization/deserialization
 */
class Player : public Account {
    private:
        vector<vector<int>> board;    ///< 5x5 BINGO board containing numbers
        vector<vector<bool>> marked;  ///< Tracks marked numbers on the board
        int gameCount;               ///< Total number of games played
        int winCount;                ///< Number of games won
        int loseCount;               ///< Number of games lost
        double winRate;              ///< Player's win rate percentage
        
    public:
        /**
         * @brief Constructor for Player class
         * @param user Username for the player
         * @param pwd Password for the player's account
         */
        Player(string user, string pwd);

        // GETTER METHODS
        /**
         * @brief Get the total number of games played
         * @return Total game count
         */
        int getGameCount() const;

        /**
         * @brief Get the number of games won
         * @return Win count
         */
        int getWinCount() const;

        /**
         * @brief Get the number of games lost
         * @return Loss count
         */
        int getLoseCount() const;

        /**
         * @brief Calculate and get the player's win rate
         * @return Win rate as a percentage
         */
        double getWinRate() const;

        /**
         * @brief Get the current board state as a JSON string
         * @return JSON string representing the board state
         */
        string getBoardState() const;

        // SETTER METHODS
        /**
         * @brief Set the board and marked states
         * @param board New board configuration
         * @param marked New marked state configuration
         */
        void setBoard(const vector<vector<int>>& board, const vector<vector<bool>>& marked);

        /**
         * @brief Set the total game count
         * @param count New game count value
         */
        void setGameCount(int count);

        /**
         * @brief Set the win count
         * @param count New win count value
         */
        void setWinCount(int count);

        /**
         * @brief Set the lose count
         * @param count New lose count value
         */
        void setLoseCount(int count);

        /**
         * @brief Set the win rate
         * @param rate New win rate value
         */
        void setWinRate(double rate);

        // DATA PROCESSING METHODS
        /**
         * @brief Convert player data to JSON string
         * @return JSON string representation of player data
         */
        string to_json() const;

        /**
         * @brief Create Player objects from JSON string
         * @param json JSON string containing player data
         * @return Vector of Player objects
         */
        static vector<Player> from_json(const string& json);

        // OTHER METHODS
        /**
         * @brief Handle player authentication process
         * @return Pointer to authenticated Player object or nullptr
         */
        static Player* authenticator();

        /**
         * @brief Create a new player in the database
         * @param p Player object to create
         * @return true if creation successful, false otherwise
         */
        static bool create(const Player& p);

        /**
         * @brief Check if player exists in database
         * @param p Player object to check
         * @return true if player exists, false otherwise
         */
        static bool check(const Player& p);

        /**
         * @brief Generate a new random BINGO board
         */
        void generateBoard();

        /**
         * @brief Display the current board state
         */
        void displayBoard() const;

        /**
         * @brief Mark a number on the board
         * @param num Number to mark
         * @return true if number found and marked, false otherwise
         */
        bool markNumber(int num);

        /**
         * @brief Check if player has won
         * @return true if player has 5 or more completed lines
         */
        bool checkWin();

        /**
         * @brief Update player statistics after a game
         * @param won true if player won, false otherwise
         */
        void updateStats(bool won);
};

#endif // PLAYER_H