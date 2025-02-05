/**
 * @file Leaderboard.cpp
 * @brief Implementation of the Leaderboard class functionality
 */

#include "../include/Leaderboard.h"
#include "../include/DB.h"
#include "../include/Util.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief Displays a formatted table of player rankings and statistics
 * 
 * This method performs the following operations:
 * 1. Loads all player records from the database
 * 2. If no records exist, displays a "No records found" message
 * 3. Sorts players by win rate in descending order
 * 4. Displays a formatted table with columns for:
 *    - Rank (position in leaderboard)
 *    - Player name
 *    - Total games played
 *    - Number of wins
 *    - Win rate percentage
 * 
 * The table is formatted using setw for consistent column widths
 * and fixed precision for win rate percentages.
 */
void Leaderboard::displayLeaderboard() const {
    cout << "\n=== Leaderboard ===\n";
    vector<Player> records = DB::getInstance().load<Player>();
    if (records.empty()) {
        cout << "No records found.\n";
        Util::showLine();
        Util::waitEnter();
        return;
    }

    // Sort players by win rate in descending order
    sort(records.begin(), records.end(), [](const Player& a, const Player& b) {
        return a.getWinRate() > b.getWinRate();
    });

    // Display table header with fixed column widths
    cout << left << setw(10) << "Rank" 
        << setw(20) << "Name" 
        << setw(15) << "Games Played"
        << setw(10) << "Wins" 
        << "Win Rate (%)" << endl;
    cout << string(65, '-') << endl;

    // Display each player's statistics in table format
    for (size_t i = 0; i < records.size(); ++i) {
        cout << left 
            << setw(10) << (i + 1)
            << setw(20) << records[i].getUsername()
            << setw(15) << records[i].getGameCount()
            << setw(10) << records[i].getWinCount()
            << fixed << setprecision(2) << records[i].getWinRate() << endl;
    }
}
