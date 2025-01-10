#include "../include/server.h"
#include "../include/client.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
using namespace std;

class Player {

private:
    string name;
    int gamesPlayed;
    int wins;
    vector<vector<int>> board;
    vector<vector<bool>> marked;

public:
    Player(string playerName) : name(playerName), gamesPlayed(0), wins(0) {
        board.resize(5, vector<int>(5));
        marked.resize(5, vector<bool>(5, false));
    }

    void generateBoard() {
        vector<int> numbers(25);
        iota(numbers.begin(), numbers.end(), 1); 
        random_device rd;  // Seed for the random number generator
        mt19937 g(rd());   // Mersenne Twister engine
        shuffle(numbers.begin(), numbers.end(),g);

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                board[i][j] = numbers[i * 5 + j];
                marked[i][j] = false;
            }
        }
    }

    void displayBoard() {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (marked[i][j]) {
                    cout << setw(3) << "X";
                } else {
                    cout << setw(3) << board[i][j];
                }
            }
            cout << endl;
        }
    }

    bool markNumber(int num) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (board[i][j] == num) {
                    marked[i][j] = true;
                    return true;
                }
            }
        }
        return false;
    }

    bool checkWin() {

        for (int i = 0; i < 5; ++i) {
            if (all_of(marked[i].begin(), marked[i].end(), [](bool m) { return m; })) return true;
            if (all_of(marked.begin(), marked.end(), [i](vector<bool>& row) { return row[i]; })) return true;
        }

        bool diagonal1 = true, diagonal2 = true;
        for (int i = 0; i < 5; ++i) {
            if (!marked[i][i]) diagonal1 = false;
            if (!marked[i][4 - i]) diagonal2 = false;
        }

        return diagonal1 || diagonal2;
    }

    void updateStats(bool won) {
        gamesPlayed++;
        if (won) wins++;
    }

    string getName(){
        return name;
    }

    int getGamesPlayed() const { return gamesPlayed; }
    int getWins() const { return wins; }
    double getWinRate() const {
        return gamesPlayed > 0 ? (static_cast<double>(wins) / gamesPlayed) * 100 : 0.0;
    }
};

class Menu {
private:
    bool isRunning;

public:
    Menu() : isRunning(true) {}

    void handleStartGame() {
        // .................
    }

    void handleViewRecords() {
        // .................
    }

    void handleSearchRecord() {
        // .................
    }

    void handleViewLeaderboard() {
        // .................
    }

    void exitProgram() {
        isRunning = false;
    }

    void displayMainMenu() {
        while (isRunning) {
            system("cls");
            cout << "\n=== Bingo Game Menu ===" << endl;
            cout << "1. Start Game" << endl;
            cout << "2. View Records" << endl;
            cout << "3. Search Record" << endl;
            cout << "4. View Leaderboard" << endl;
            cout << "5. Exit" << endl;
            cout << "Choose (1-5): ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: handleStartGame(); break;
                case 2: handleViewRecords(); break;
                case 3: handleSearchRecord(); break;
                case 4: handleViewLeaderboard(); break;
                case 5: exitProgram(); break;
                case 6: testServer(); break;
                case 7: testClient(); break;
                default: break;
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
        }
    }
};

int main() {
    Menu menu;
    menu.displayMainMenu();
    return 0;
}