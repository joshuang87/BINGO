/**
 * @file Leaderboard.h
 * @brief Header file for the Leaderboard class that manages player rankings
 */

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "Player.h"
#include <vector>
#include <string>

using namespace std;

/**
 * @class Leaderboard
 * @brief Class responsible for displaying and managing player rankings
 * 
 * The Leaderboard class maintains a collection of player records and provides
 * functionality to display them in a sorted order based on win rates.
 */
class Leaderboard {
    public:
        /**
         * @brief Vector containing player records to be displayed
         */
        vector<Player> records;

        /**
         * @brief Displays the leaderboard with player rankings and statistics
         * 
         * This method shows a formatted table of players sorted by win rate,
         * including their rank, name, games played, wins, and win rate.
         * If no records are found, it displays an appropriate message.
         */
        void displayLeaderboard() const;
};

#endif