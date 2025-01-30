#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "Player.h"
#include <vector>
#include <string>

class Leaderboard {
    public:
        std::vector<Player> records;
        void displayLeaderboard() const;
};

#endif