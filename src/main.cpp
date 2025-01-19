#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
#include <set>

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
        random_device rd;
        mt19937 g(rd());
        shuffle(numbers.begin(), numbers.end(), g);

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                board[i][j] = numbers[i * 5 + j];
                marked[i][j] = false;
            }
        }
    }

    void displayBoard() const {
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
        int completedLines = 0;
    
        // check rows
        for (int i = 0; i < 5; ++i) {
            if (all_of(marked[i].begin(), marked[i].end(), [](bool m) { return m; })) {
                completedLines++;
            }
        }
        
        // check columns
        for (int i = 0; i < 5; ++i) {
            bool columnComplete = true;
            for (int j = 0; j < 5; ++j) {
                if (!marked[j][i]) {
                    columnComplete = false;
                    break;
                }
            }
            if (columnComplete) {
                completedLines++;
            }
        }
        
        // check first diagonal (top-left to bottom-right)
        bool diagonal1Complete = true;
        for (int i = 0; i < 5; ++i) {
            if (!marked[i][i]) {
                diagonal1Complete = false;
                break;
            }
        }
        if (diagonal1Complete) {
            completedLines++;
        }
        
        // Check second diagonal (top-right to bottom-left)
        bool diagonal2Complete = true;
        for (int i = 0; i < 5; ++i) {
            if (!marked[i][4 - i]) {
                diagonal2Complete = false;
                break;
            }
        }
        if (diagonal2Complete) {
            completedLines++;
        }
        
        // return true if 5 or more lines are completed
        return completedLines >= 5;
    }

    void updateStats(bool won) {
        gamesPlayed++;
        if (won) wins++;
    }

    string getName() const { return name; }
    int getGamesPlayed() const { return gamesPlayed; }
    int getWins() const { return wins; }
    double getWinRate() const {
        return gamesPlayed > 0 ? (static_cast<double>(wins) / gamesPlayed) * 100 : 0.0;
    }
};

class Game {
private:
    vector<Player> players;
    int currentTurn;
    bool isOver;
    set<int> usedNumbers;

public:
    Game() : currentTurn(0), isOver(false) {}

    void startGame(vector<string> playerNames) { //check if player is 2
        if (playerNames.size() != 2) {
            cout << "This game requires exactly 2 players.\n";
            return;
        }

        players.clear();
        for (const auto& name : playerNames) { //generate board for each player and save their name to player<>
            Player player(name);
            player.generateBoard();
            players.push_back(player);
        }

        currentTurn = 0;
        isOver = false;
        cout << "Game started between " << players[0].getName() << " and " << players[1].getName() << ".\n";

        for (const auto& player : players) {
            cout << player.getName() << "'s board:\n";
            player.displayBoard();
            cout << endl;
        }
    }

    void playTurn() {
        if (isOver) {
            cout << "Game is already over.\n";
            return;
        }

        Player& currentPlayer = players[currentTurn];

        system("cls"); 

        cout << currentPlayer.getName() << "'s turn.\n";
        cout << "Your board:\n";
        currentPlayer.displayBoard();

        int number;
        cout << "Enter a number to mark (1-25): ";
        cin >> number;

        if (number < 1 || number > 25) {
            cout << "Invalid number. Please choose a number between 1 and 25.\n";
            cin.ignore();
            cin.get();
            return;
        }
        
        // Check if number was already used
        if (usedNumbers.find(number) != usedNumbers.end()) {
            cout << "Number " << number << " was already used. Try another number.\n";
            cin.ignore();
            cin.get();
            return;
        }
        bool numberMarked = false;
        for (auto& player : players) {
            if (player.markNumber(number)) {
                numberMarked = true;
            }
        }

        if (numberMarked) {
            usedNumbers.insert(number);  // Add to used numbers set
        }

        /*if (!numberMarked) {
            cout << "Number " << number << " was already marked. Try another number.\n";
            cin.ignore();
            cin.get();
            return;
        }*/

        cout << currentPlayer.getName() << "'s board after marking " << number << ":\n";
        currentPlayer.displayBoard();

        // check if any player has won after the number is marked
        bool gameWon = false;
        Player* winner = nullptr;
        
        for (auto& player : players) {
            if (player.checkWin()) {
                gameWon = true;
                winner = &player;
                break;
            }
        }

        if (gameWon && winner != nullptr) {
            cout << winner->getName() << " wins!\n";
            isOver = true;
            
            // update stats for all players
            for (auto& player : players) {
                player.updateStats(player.getName() == winner->getName());
            }
            
            cout << "\nFinal boards:\n";
            for (const auto& player : players) {
                cout << player.getName() << "'s board:\n";
                player.displayBoard();
                cout << endl;
            }
            return;
        }

        cout << "\nPress Enter to continue to the next player...\n";
        cin.ignore();
        cin.get();

        currentTurn = (currentTurn + 1) % players.size();
    }

    bool checkWinCondition() {
        return players[currentTurn].checkWin();
    }

};

class Leaderboard {
private:
    vector<Player> records;

public:
    void updateLeaderboard(const vector<Player>& players) {
        records = players;
        sort(records.begin(), records.end(), [](const Player& a, const Player& b) {
            if (a.getWinRate() == b.getWinRate())
                return a.getWins() > b.getWins();
            return a.getWinRate() > b.getWinRate();
        });
    }

    void displayLeaderboard() {
        cout << "\n=== Leaderboard ===\n";
        cout << left << setw(10) << "Rank" << setw(20) << "Name" << setw(10) << "Wins" << "Win Rate (%)" << endl;
        for (size_t i = 0; i < records.size(); ++i) {
            cout << left << setw(10) << (i + 1) << setw(20) << records[i].getName() << setw(10) << records[i].getWins()
                 << fixed << setprecision(2) << records[i].getWinRate() << endl;
        }
    }
};

class Menu {
private:
    bool isRunning;
    vector<Player> players;
    Leaderboard leaderboard;
    Game game;

public:
    Menu() : isRunning(true) {}
     // Check if a name already exists
    bool isNameTaken(const string& name) {
        return find_if(players.begin(), players.end(),
            [&name](const Player& p) { return p.getName() == name; }) != players.end();
    }

    void handleStartGame() {
        system("cls");
        cout << "\n=== Start New Game ===" << endl;
        
        // Get player names
       vector<string> playerNames;
        for (int i = 0; i < 2; i++) {
            string name;
            bool validName = false;
            
            while (!validName) {
                cout << "Enter name for Player " << (i + 1) << ": ";
                cin >> name;
            
              // Check if this name is already taken by another player
                if (find(playerNames.begin(), playerNames.end(), name) != playerNames.end() || 
                    (isNameTaken(name) && find(playerNames.begin(), playerNames.end(), name) == playerNames.end())) {
                    cout << "This name is already taken. Please choose a different name.\n";
                } else {
                    validName = true;
                    playerNames.push_back(name);
                    if (!isNameTaken(name)) {
                        players.push_back(Player(name));
                    }
                }
            }
        }

        // Start the game
        game.startGame(playerNames);
        
        // Game loop
        while (!game.checkWinCondition()) {
            game.playTurn();
        }
        
        // Update leaderboard after game ends
        leaderboard.updateLeaderboard(players);
        cout << "\nPress any key to return to menu...";
        cin.ignore();
        cin.get();
    }

    void handleViewRecords() {
        system("cls");
        cout << "\n=== Player Records ===" << endl;
        cout << left << setw(20) << "Name" << setw(15) << "Games Played" 
             << setw(10) << "Wins" << "Win Rate (%)" << endl;
        cout << string(55, '-') << endl;

        for (const auto& player : players) {
            cout << left << setw(20) << player.getName() 
                 << setw(15) << player.getGamesPlayed()
                 << setw(10) << player.getWins()
                 << fixed << setprecision(2) << player.getWinRate() << endl;
        }

        cout << "\nPress any key to return to menu...";
        cin.ignore();
        cin.get();
    }

    void handleSearchRecord() {
        system("cls");
        cout << "\n=== Search Player Record ===" << endl;
        
        string searchName;
        cout << "Enter player name to search: ";
        cin >> searchName;

        auto player = find_if(players.begin(), players.end(),
            [&searchName](const Player& p) { return p.getName() == searchName; });

        if (player != players.end()) {
            cout << "\nPlayer found!" << endl;
            cout << "Name: " << player->getName() << endl;
            cout << "Games Played: " << player->getGamesPlayed() << endl;
            cout << "Wins: " << player->getWins() << endl;
            cout << "Win Rate: " << fixed << setprecision(2) << player->getWinRate() << "%" << endl;
        } else {
            cout << "\nPlayer not found!" << endl;
        }

        cout << "\nPress any key to return to menu...";
        cin.ignore();
        cin.get();
    }

    void handleViewLeaderboard() {
        leaderboard.updateLeaderboard(players);
        leaderboard.displayLeaderboard();
        cout << "\nPress any key to return to menu...";
        cin.ignore();
        cin.get();
    }

    void exitProgram() {
        isRunning = false;
        cout << "\nThank you for playing! Goodbye!\n";
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
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    system("pause");
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