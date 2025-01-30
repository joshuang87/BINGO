#include "../include/Leaderboard.h"
#include "../include/DB.h"
#include "../include/Util.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

void Leaderboard::displayLeaderboard() const {
    cout << "\n=== Leaderboard ===\n";
    vector<Player> records = DB::getInstance().load<Player>();
    if (records.empty()) {
        cout << "No records found.\n";
        Util::showLine();
        Util::waitEnter();
        return;
    }

    sort(records.begin(), records.end(), [](const Player& a, const Player& b) {
        return a.getWinRate() > b.getWinRate();
    });

    cout << left << setw(10) << "Rank" 
        << setw(20) << "Name" 
        << setw(15) << "Games Played"
        << setw(10) << "Wins" 
        << "Win Rate (%)" << endl;
    cout << string(65, '-') << endl;

    for (size_t i = 0; i < records.size(); ++i) {
        cout << left 
            << setw(10) << (i + 1)
            << setw(20) << records[i].getUsername()
            << setw(15) << records[i].getGameCount()
            << setw(10) << records[i].getWinCount()
            << fixed << setprecision(2) << records[i].getWinRate() << endl;
    }
}
