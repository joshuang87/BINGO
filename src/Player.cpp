/**
 * @file Player.cpp
 * @brief Implementation of the Player class functionality
 */

#include "../include/Util.h"
#include "../include/Account.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include "../include/DB.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <numeric>
#include <sstream>

/**
 * @brief Constructor initializes a new Player with default values
 * @param user Username for the player
 * @param pwd Password for the player's account
 */
Player::Player(string user, string pwd)
    : Account(user, pwd), gameCount(0), winCount(0), loseCount(0) {
    board.resize(5, vector<int>(5));
    marked.resize(5, vector<bool>(5, false));
}

#pragma region Getter

/**
 * @brief Get the total number of games played
 * @return Total game count
 */
int Player::getGameCount() const {
    return gameCount;
}

/**
 * @brief Get the number of games won
 * @return Win count
 */
int Player::getWinCount() const {
    return winCount;
}

/**
 * @brief Calculate and get the player's win rate
 * @return Win rate as a percentage
 */
double Player::getWinRate() const {
    return gameCount == 0 ? 0.0 : (static_cast<double>(winCount) / gameCount) * 100;
}

/**
 * @brief Get the number of games lost
 * @return Loss count
 */
int Player::getLoseCount() const {
    return loseCount;
}

/**
 * @brief Get the current board state as a JSON string
 * @return JSON string representing the board state
 */
string Player::getBoardState() const {
    string state = "[";
    for (int i = 0; i < 5; ++i) {
        state += "[";
        for (int j = 0; j < 5; ++j) {
            if (marked[i][j]) {
                state += "\"x\"";
            } else {
                state += "\"" + to_string(board[i][j]) + "\"";
            }
            if (j < 4) state += ", ";
        }
        state += "]";
        if (i < 4) state += ",";
    }
    state += "]";
    return state;
}

#pragma endregion

#pragma region Setter

/**
 * @brief Set the board and marked states
 * @param board New board configuration
 * @param marked New marked state configuration
 */
void Player::setBoard(const vector<vector<int>>& board, const vector<vector<bool>>& marked) {
    this->board = board;
    this->marked = marked;
}

/**
 * @brief Set the total game count
 * @param count New game count value
 */
void Player::setGameCount(int count) {
    this->gameCount = count;
}

/**
 * @brief Set the win count
 * @param count New win count value
 */
void Player::setWinCount(int count) {
    this->winCount = count;
}

/**
 * @brief Set the lose count
 * @param count New lose count value
 */
void Player::setLoseCount(int count) {
    this->loseCount = count;
}

/**
 * @brief Set the win rate
 * @param rate New win rate value
 */
void Player::setWinRate(double rate) {
    this->winRate = rate;
}

#pragma endregion

#pragma region Validator

/**
 * @brief Handle player authentication process
 * 
 * This method:
 * 1. Displays login/signup options
 * 2. Handles user input for authentication
 * 3. Validates credentials
 * 4. Creates or loads player data
 * 
 * @return Pointer to authenticated Player object or nullptr
 */
Player* Player::authenticator() {
    string choice, name, password;
    int input;

    do {
        cout << "1. SignIn" << endl;
        cout << "2. SignUp" << endl;
        Util::showLine();
        cout << "Choose (1-2): ";
        cin >> choice;

        if (!Util::isNumber(choice)) {
            cout << "Invalid input! Please enter again.\n";
            Util::showLine();
            Util::waitEnter();
            continue;
        }
        input = stoi(choice);
        if (input < 1 || input > 2) {
            cout << "Invalid choice input! Please enter again.\n";
            Util::showLine();
            Util::waitEnter();
            continue;
        }
        break;
    } while (true);

    do {
        if (input == 1) {
            system("cls");
            cout << "===== SignIn =====" << endl;
            cout << "Name : ";
            cin >> name;
            cout << "Password : ";
            cin >> password;

            Player* p = new Player(name, password);
            if (Player::check(*p)) {
                system("cls");
                // Load existing player data
                vector<Player> players = DB::getInstance().load<Player>();
                for (const Player& player : players) {
                    if (player.getUsername() == name && player.getPassword() == password) {
                        delete p;  // Delete temporary object
                        return new Player(player);  // Return new object with all data
                    }
                }
            }
            cout << "Invalid username or password!\n";
            LOG_INFO("Account not available");
            delete p;
            Util::waitEnter();
            return authenticator();
        }
        else if (input == 2) {
            system("cls");
            cout << "===== SignUp =====" << endl;
            cout << "Name : ";
            cin >> name;
            cout << "Password : ";
            cin >> password;

            Player* p = new Player(name, password);
            if (Player::create(*p)) {
                system("cls");
                return p;
            } else {
                LOG_ERROR("Account creation failed!");
                delete p;
                return authenticator();
            }
        }
    } while (true);

    system("cls");
    return nullptr;  // Should never reach here
}

/**
 * @brief Check if player has won by counting completed lines
 * @return true if player has 5 or more completed lines
 */
bool Player::checkWin() {
    int completedLines = 0;

    // check rows
    for (int i = 0; i < 5; ++i) {
        if (std::all_of(marked[i].begin(), marked[i].end(), [](bool m) { return m; })) {
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
    
    return completedLines >= 5;
}

/**
 * @brief Check if player exists in database
 * @param p Player object to check
 * @return true if player exists with matching credentials
 */
bool Player::check(const Player& p) {
    vector<Player> players = DB::getInstance().load<Player>();
    if (players.empty()) return false;

    for (const Player& player : players) {
        if (player.getUsername() == p.getUsername() && player.getPassword() == p.getPassword()) return true;
    }
    return false;
}

#pragma endregion

#pragma region Data Processing

/**
 * @brief Update player statistics after a game
 * @param won true if player won, false if lost
 */
void Player::updateStats(bool won) {
    gameCount++;
    if (won) winCount++;
    else loseCount++;
    winRate = gameCount == 0 ? 0.0 : (static_cast<double>(winCount) / gameCount) * 100;
}

/**
 * @brief Create Player objects from JSON string
 * @param json JSON string containing player data
 * @return Vector of Player objects
 */
vector<Player> Player::from_json(const string& json) {
    vector<Player> players;
    stringstream ss(json);
    string line;

    // Remove the opening and closing brackets of the JSON array
    getline(ss, line, '[');
    
    while (getline(ss, line, '}')) {
        // Skip empty entries
        if (line.find("username") == string::npos) continue;

        // Remove the comma and opening brace
        if (line[0] == ',') line = line.substr(1);
        if (line[0] == '{') line = line.substr(1);

        size_t pos;
        
        // Parse username
        pos = line.find("username");
        string username = line.substr(pos + 11); // skip "username":"
        username = username.substr(0, username.find("\""));

        // Parse password
        pos = line.find("password");
        string password = line.substr(pos + 11); // skip "password":"
        password = password.substr(0, password.find("\""));

        // Create player object
        Player player(username, password);

        // Parse gameCount
        pos = line.find("gameCount");
        string gameCount = line.substr(pos + 11); // skip "gameCount":
        player.gameCount = stoi(gameCount.substr(0, gameCount.find(",")));

        // Parse winCount
        pos = line.find("winCount");
        string winCount = line.substr(pos + 10); // skip "winCount":
        player.winCount = stoi(winCount.substr(0, winCount.find(",")));

        // Parse loseCount
        pos = line.find("loseCount");
        string loseCount = line.substr(pos + 11); // skip "loseCount":
        player.loseCount = stoi(loseCount.substr(0, loseCount.find(",")));

        // Parse winRate
        pos = line.find("winRate");
        string winRate = line.substr(pos + 9); // skip "winRate":
        player.winRate = stod(winRate.substr(0, winRate.find(",")));

        players.push_back(player);
    }

    return players;
}

/**
 * @brief Convert player data to JSON string
 * @return JSON string representation of player data
 */
string Player::to_json() const {
    return "{\"username\":\"" + getUsername() + "\",\"password\":\"" + getPassword() + "\",\"gameCount\":" + to_string(gameCount) + ",\"winCount\":" + to_string(winCount) + ",\"loseCount\":" + to_string(loseCount) + ",\"winRate\":" + to_string(getWinRate()) + "}";
}

/**
 * @brief Create a new player in the database
 * @param p Player object to create
 * @return true if creation successful, false if player already exists
 */
bool Player::create(const Player& p) {
    if (Player::check(p)) return false;
    return DB::getInstance().save(p);
}

#pragma endregion

#pragma region Other

/**
 * @brief Generate a new random BINGO board
 * 
 * Creates a 5x5 board with random numbers 1-25,
 * ensuring no duplicates.
 */
void Player::generateBoard() {
    std::vector<int> numbers(25);
    std::iota(numbers.begin(), numbers.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(numbers.begin(), numbers.end(), g);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = numbers[i * 5 + j];
            marked[i][j] = false;
        }
    }
}

/**
 * @brief Display the current board state
 * 
 * Shows the board with marked numbers displayed as 'X'
 * and unmarked numbers shown as is.
 */
void Player::displayBoard() const {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (marked[i][j]) {
                std::cout << std::setw(3) << "X";
            } else {
                std::cout << std::setw(3) << board[i][j];
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Mark a number on the board
 * @param num Number to mark
 * @return true if number found and marked, false if not found
 */
bool Player::markNumber(int num) {
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

#pragma endregion