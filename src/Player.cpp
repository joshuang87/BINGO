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

Player::Player(std::string user, std::string pwd)
    : Account(user, pwd), gameCount(0), winCount(0), loseCount(0) {
    board.resize(5, std::vector<int>(5));
    marked.resize(5, std::vector<bool>(5, false));
}

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
                // 加载已存在玩家的所有数据
                vector<Player> players = DB::getInstance().load<Player>();
                for (const Player& player : players) {
                    if (player.getUsername() == name && player.getPassword() == password) {
                        delete p;  // 删除临时对象
                        return new Player(player);  // 返回包含所有数据的新对象
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

void Player::setBoard(const vector<vector<int>>& board, const vector<vector<bool>>& marked) {
    this->board = board;
    this->marked = marked;
}

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

void Player::updateStats(bool won) {
    gameCount++;
    if (won) winCount++;
    else loseCount++;
    winRate = gameCount == 0 ? 0.0 : (static_cast<double>(winCount) / gameCount) * 100;
}

int Player::getGameCount() const {
    return gameCount;
}

int Player::getWinCount() const {
    return winCount;
}

double Player::getWinRate() const {
    return gameCount == 0 ? 0.0 : (static_cast<double>(winCount) / gameCount) * 100;
}

int Player::getLoseCount() const {
    return loseCount;
}

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

string Player::to_json() const {
    return "{\"username\":\"" + getUsername() + "\",\"password\":\"" + getPassword() + "\",\"gameCount\":" + to_string(gameCount) + ",\"winCount\":" + to_string(winCount) + ",\"loseCount\":" + to_string(loseCount) + ",\"winRate\":" + to_string(getWinRate()) + "}";
}

bool Player::check(const Player& p) {
    vector<Player> players = DB::getInstance().load<Player>();
    if (players.empty()) return false;

    for (const Player& player : players) {
        if (player.getUsername() == p.getUsername() && player.getPassword() == p.getPassword()) return true;
    }
    return false;
}

bool Player::create(const Player& p) {
    if (Player::check(p)) return false;
    return DB::getInstance().save(p);
}