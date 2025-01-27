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
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <map>
#include <unordered_map>
#include <conio.h>
#include <filesystem>

using namespace std;

string getHiddenInput() {
    string input;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!input.empty()) {
                cout << "\b \b";
                input.pop_back();
            }
        } else {
            input += ch;
            cout << '*';
        }
    }
    cout << endl;
    return input;
}

void displayRules() {
    cout << "\n=== Bingo Game Rules ===" << endl;
    cout << "1. Each player gets a 5x5 board with numbers 1-25 randomly arranged" << endl;
    cout << "2. Players take turns calling numbers from 1-25" << endl;
    cout << "3. If the called number appears on any player's board, it's marked with 'X'" << endl;
    cout << "4. To win, a player must complete 5 lines (horizontal, vertical, or diagonal)" << endl;
    cout << "5. A player can only see their own board during the game" << endl;
    cout << "6. Type a number (1-25) to play or 'Q' to quit during your turn" << endl;
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

// User class to handle authentication
class User {
private:
    string username;
    string password;
    string favoriteColor;
    string favoriteSport;

public:
    User() {}
    User(string uname, string pwd, string color, string sport) 
        : username(uname), password(pwd), favoriteColor(color), favoriteSport(sport) {}

    string getUsername() const { return username; }
    bool verifyPassword(const string& pwd) const { return password == pwd; }
    bool verifySecurityAnswers(const string& color, const string& sport) const {
        return favoriteColor == color && favoriteSport == sport;
    }
    bool verifyColor(const string& color) const {
        return favoriteColor == color;
    }
    bool verifySport(const string& sport) const {
        return favoriteSport == sport;
    }
    void changePassword(const string& newPwd) { password = newPwd; }
    
    string serialize() const {
        return username + "," + password + "," + favoriteColor + "," + favoriteSport;
    }
    
    void deserialize(const string& data) {
        stringstream ss(data);
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, favoriteColor, ',');
        getline(ss, favoriteSport, ',');
    }
};

// UserManager class to handle user operations
class UserManager {
private:
    unordered_map<string, User> users;
    const string USER_FILE = "users.txt";

public:

     bool verifyColor(const string& username, const string& color) {
        auto it = users.find(username);
        return it != users.end() && it->second.verifyColor(color);
    }

    bool verifySport(const string& username, const string& sport) {
        auto it = users.find(username);
        return it != users.end() && it->second.verifySport(sport);
    }
    
    UserManager() {
        loadUsers();
    }

    void loadUsers() {
        ifstream file(USER_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            User user;
            user.deserialize(line);
            users[user.getUsername()] = user;
        }
        file.close();
    }

    void saveUsers() {
        ofstream file(USER_FILE);
        if (!file.is_open()) {
            cout << "Error saving users\n";
            return;
        }

        for (const auto& pair : users) {
            file << pair.second.serialize() << "\n";
        }
        file.close();
    }

    bool registerUser(const string& username, const string& password,
                     const string& color, const string& sport) {
        if (users.find(username) != users.end()) {
            return false;
        }
        users[username] = User(username, password, color, sport);
        saveUsers();
        return true;
    }

    bool authenticateUser(const string& username, const string& password) {
        auto it = users.find(username);
        return it != users.end() && it->second.verifyPassword(password);
    }

    bool resetPassword(const string& username, const string& color,
                      const string& sport, const string& newPassword) {
        auto it = users.find(username);
        if (it != users.end() && it->second.verifySecurityAnswers(color, sport)) {
            it->second.changePassword(newPassword);
            saveUsers();
            return true;
        }
        return false;
    }

    bool userExists(const string& username) {
        return users.find(username) != users.end();
    }
};

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
       loadPlayerStats();
   }

   string getName() const { return name; }
   int getGamesPlayed() const { return gamesPlayed; }
   int getWins() const { return wins; }
   double getWinRate() const {
       return gamesPlayed > 0 ? (static_cast<double>(wins) / gamesPlayed) * 100 : 0.0;
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

       // Check rows
       for (int i = 0; i < 5; ++i) {
           if (all_of(marked[i].begin(), marked[i].end(), [](bool m) { return m; })) {
               completedLines++;
           }
       }

       // Check columns
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

       // Check diagonals
       bool diagonal1Complete = true;
       bool diagonal2Complete = true;
       for (int i = 0; i < 5; ++i) {
           if (!marked[i][i]) diagonal1Complete = false;
           if (!marked[i][4-i]) diagonal2Complete = false;
       }
       if (diagonal1Complete) completedLines++;
       if (diagonal2Complete) completedLines++;

       return completedLines >= 5;
   }

   void updateStats(bool won) {
       gamesPlayed++;
       if (won) wins++;
       savePlayerStats();
   }

   void setStats(int games, int winCount) {
       gamesPlayed = games;
       wins = winCount;
   }

 void savePlayerStats() {
    ofstream tempFile("temp_stats.txt");
    ifstream statsFile("player_stats.txt");
    
    string line;
    while (getline(statsFile, line)) {
        stringstream ss(line);
        string playerName;
        getline(ss, playerName, ',');
        
        if (playerName != name) {
            tempFile << line << endl;
        }
    }
    
    tempFile << name << "," << gamesPlayed << "," << wins << endl;
    
    tempFile.close();
    statsFile.close();
    
    remove("player_stats.txt");
    rename("temp_stats.txt", "player_stats.txt");
}

   void loadPlayerStats() {
       ifstream file("player_stats.txt");
       if (file.is_open()) {
           string line;
           while (getline(file, line)) {
               stringstream ss(line);
               string playerName;
               getline(ss, playerName, ',');
               
               if (playerName == name) {
                   string gamesStr, winsStr;
                   getline(ss, gamesStr, ',');
                   getline(ss, winsStr, ',');
                   gamesPlayed = stoi(gamesStr);
                   wins = stoi(winsStr);
                   break;
               }
           }
           file.close();
       }
   }

   string serializePlayerData() const {
       stringstream ss;
       ss << name << "," << gamesPlayed << "," << wins;
       for (const auto& row : board) {
           for (int num : row) ss << "," << num;
       }
       for (const auto& row : marked) {
           for (bool mark : row) ss << "," << (mark ? "1" : "0");
       }
       return ss.str();
   }

   void deserializePlayerData(const string& data) {
       stringstream ss(data);
       string item;
       getline(ss, name, ',');
       getline(ss, item, ','); gamesPlayed = stoi(item);
       getline(ss, item, ','); wins = stoi(item);

       board.resize(5, vector<int>(5));
       for (int i = 0; i < 5; ++i) {
           for (int j = 0; j < 5; ++j) {
               getline(ss, item, ',');
               board[i][j] = stoi(item);
           }
       }

       marked.resize(5, vector<bool>(5));
       for (int i = 0; i < 5; ++i) {
           for (int j = 0; j < 5; ++j) {
               getline(ss, item, ',');
               marked[i][j] = (item == "1");
           }
       }
   }
};

class RoomManager {
private:
    vector<string> activeRooms;
    const string SAVE_DIRECTORY = "saved_games/";

public:
    RoomManager() {
        if (!filesystem::exists(SAVE_DIRECTORY)) {
            filesystem::create_directory(SAVE_DIRECTORY);
        }
        loadActiveRooms();
    }

    void loadActiveRooms() {
        activeRooms.clear();
        for (const auto& entry : filesystem::directory_iterator(SAVE_DIRECTORY)) {
            if (entry.path().extension() == ".txt") {
                string filename = entry.path().filename().string();
                activeRooms.push_back(filename.substr(0, filename.length() - 4));
            }
        }
    }

    string generateRoomId() {
        int nextId = 1;
        while (true) {
            string roomId = "Room_" + to_string(nextId);
            if (find(activeRooms.begin(), activeRooms.end(), roomId) == activeRooms.end()) {
                return roomId;
            }
            nextId++;
        }
    }

    void addRoom(const string& roomId) {
        if (find(activeRooms.begin(), activeRooms.end(), roomId) == activeRooms.end()) {
            activeRooms.push_back(roomId);
        }
    }

    void removeRoom(const string& roomId) {
        string filename = SAVE_DIRECTORY + roomId + ".txt";
        if (filesystem::exists(filename)) {
            filesystem::remove(filename);
        }
        auto it = find(activeRooms.begin(), activeRooms.end(), roomId);
        if (it != activeRooms.end()) {
            activeRooms.erase(it);
        }
    }

    const vector<string>& getActiveRooms() const {
        return activeRooms;
    }

    string getSaveFilePath(const string& roomId) const {
        return SAVE_DIRECTORY + roomId + ".txt";
    }
};

class Game {
public:
    static RoomManager roomManager;

private:
    vector<Player> players;
    int currentTurn;
    bool isOver;
    set<int> usedNumbers;
    string gameSessionId;
    string roomName;

public:
    Game() : currentTurn(0), isOver(false) {
        gameSessionId = to_string(chrono::system_clock::to_time_t(chrono::system_clock::now()));
        roomName = roomManager.generateRoomId();
    }

    bool isGameOver() const { return isOver; }
    string getRoomName() const { return roomName; }
    void setRoomName(const string& name) { roomName = name; }
    const vector<Player>& getPlayers() const { return players; }

    void saveGameState() {
        string filename = roomManager.getSaveFilePath(roomName);
        ofstream saveFile(filename);
        if (!saveFile.is_open()) {
            cout << "Error: Could not open save file.\n";
            return;
        }

        saveFile << gameSessionId << "\n" << roomName << "\n"
                << currentTurn << "\n" << isOver << "\n"
                << usedNumbers.size() << "\n";
        
        for (int num : usedNumbers) {
            saveFile << num << " ";
        }
        saveFile << "\n" << players.size() << "\n";
        
        for (const auto& player : players) {
            saveFile << player.serializePlayerData() << "\n";
        }

        saveFile.close();
        roomManager.addRoom(roomName);
        cout << "Game saved successfully in room: " << roomName << "\n";
    }

    bool loadGameState(const string& roomId) {
        string filename = roomManager.getSaveFilePath(roomId);
        ifstream loadFile(filename);
        if (!loadFile.is_open()) {
            cout << "Error: Could not open save file.\n";
            return false;
        }

        getline(loadFile, gameSessionId);
        getline(loadFile, roomName);
        
        string line;
        getline(loadFile, line); currentTurn = stoi(line);
        getline(loadFile, line); isOver = (line == "1");

        int numCount;
        loadFile >> numCount;
        usedNumbers.clear();
        for (int i = 0; i < numCount; ++i) {
            int num;
            loadFile >> num;
            usedNumbers.insert(num);
        }

        loadFile.ignore();
        int playerCount;
        loadFile >> playerCount;
        loadFile.ignore();

        players.clear();
        string playerData;
        for (int i = 0; i < playerCount; ++i) {
            getline(loadFile, playerData);
            Player player("");
            player.deserializePlayerData(playerData);
            players.push_back(player);
        }

        cout << "Game loaded successfully from room: " << roomId << "\n";
        return true;
    }

    void cleanupRoom() {
        if (!isOver) {
            char saveChoice;
            cout << "Do you want to save the game state? (Y/N): ";
            cin >> saveChoice;
            if (toupper(saveChoice) == 'Y') {
                saveGameState();
            } else {
                roomManager.removeRoom(roomName);
            }
        }
    }

    static void displaySavedGames() {
    const vector<string>& rooms = roomManager.getActiveRooms();
    if (rooms.empty()) {
        cout << "No saved games found.\n";
        return;
    }

    cout << "\nAvailable saved games:\n";
    cout << string(50, '=') << endl;
    cout << left << setw(10) << "Number" << setw(20) << "Room Name" << endl;
    cout << string(50, '-') << endl;
    
    for (size_t i = 0; i < rooms.size(); ++i) {
        cout << left << setw(10) << (i + 1) << setw(20) << rooms[i] << endl;
    }
    cout << string(50, '=') << endl;
    cout << "\nPlease enter the Number (1-" << rooms.size() << ") to select a game." << endl;
}

    static bool loadGameByIndex(Game& game, int index) {
        const vector<string>& rooms = roomManager.getActiveRooms();
        if (index < 1 || index > static_cast<int>(rooms.size())) {
            return false;
        }
        return game.loadGameState(rooms[index - 1]);
    }

    void startGame(vector<string>& playerNames) {
        if (playerNames.size() != 2) {
            cout << "This game requires exactly 2 players.\n";
            return;
        }

        players.clear();
        usedNumbers.clear();
        for (const auto& name : playerNames) {
            players.push_back(Player(name));
            players.back().generateBoard();
        }

        currentTurn = 0;
        isOver = false;
        
        cout << "Game Session ID: " << gameSessionId << endl;
        cout << "Room Name: " << roomName << endl;
        cout << "Game started between " << players[0].getName() 
             << " and " << players[1].getName() << ".\n\n";

        cout << players[currentTurn].getName() << "'s board:\n";
        players[currentTurn].displayBoard();
    }

    void playTurn() {
        if (isOver) {
            cout << "Game is already over.\n";
            return;
        }

        Player& currentPlayer = players[currentTurn];
        system("cls");

        cout << "Room Name: " << roomName << endl;
        cout << currentPlayer.getName() << "'s turn.\n";
        cout << "Your board:\n";
        currentPlayer.displayBoard();

        string input;
        cout << "\nEnter a number (1-25) or 'Q' to quit: ";
        cin >> input;

        if (toupper(input[0]) == 'Q') {
            cleanupRoom();
            isOver = true;
            return;
        }

        int number;
        try {
            number = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a number between 1-25 or 'Q' to quit.\n";
            cin.ignore();
            cin.get();
            return;
        }

        if (number < 1 || number > 25) {
            cout << "Invalid number. Please choose a number between 1 and 25.\n";
            cin.ignore();
            cin.get();
            return;
        }
        
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
            usedNumbers.insert(number);
        }

        system("cls");
        cout << "Room Name: " << roomName << endl;
        cout << currentPlayer.getName() << "'s board after marking " << number << ":\n";
        currentPlayer.displayBoard();

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
            cout << "\n" << winner->getName() << " wins!\n";
            isOver = true;

            for (auto& player : players) {
                player.updateStats(player.getName() == winner->getName());
            }

            cout << "\nFinal boards:\n";
            for (const auto& player : players) {
                cout << "\n" << player.getName() << "'s board:\n";
                player.displayBoard();
            }
            cin.ignore();
            cin.get();
            return;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();

        system("cls");
        cout << "Press Enter twice to continue...";
        cin.ignore();
        cin.get();

        currentTurn = (currentTurn + 1) % players.size();

        system("cls");
        cout << "Room Name: " << roomName << endl;
        cout << players[currentTurn].getName() << "'s turn.\n";
        cout << "Your board:\n";
        players[currentTurn].displayBoard();
    }
};

class Leaderboard {
public:
    vector<Player> records;
    const string LEADERBOARD_FILE = "leaderboard.txt";

    void loadLeaderboard() {
        records.clear();
        ifstream file(LEADERBOARD_FILE);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            string gamesStr, winsStr;
            
            getline(ss, name, ',');
            getline(ss, gamesStr, ',');
            getline(ss, winsStr, ',');

            Player player(name);
            player.setStats(stoi(gamesStr), stoi(winsStr));
            records.push_back(player);
        }
        file.close();
    }
    void saveLeaderboard() {
        ofstream file(LEADERBOARD_FILE);
        if (!file.is_open()) {
            cout << "Could not open leaderboard file for writing.\n";
            return;
        }

        for (const auto& player : records) {
            file << player.getName() << ","
                 << player.getGamesPlayed() << ","
                 << player.getWins() << "\n";
        }
        file.close();
    }

    void updateLeaderboard(const vector<Player>& players) {
        for (const auto& player : players) {
            auto it = find_if(records.begin(), records.end(),
                [&](const Player& p) { return p.getName() == player.getName(); });
            
            if (it != records.end()) {
                records.erase(it);
            }
            records.push_back(player);
        }

        sort(records.begin(), records.end(), 
            [](const Player& a, const Player& b) {
                if (a.getWinRate() == b.getWinRate())
                    return a.getWins() > b.getWins();
                return a.getWinRate() > b.getWinRate();
            });
    }

    void displayLeaderboard() const {
        cout << "\n=== Leaderboard ===\n";
        cout << left << setw(10) << "Rank" 
             << setw(20) << "Name" 
             << setw(15) << "Games Played"
             << setw(10) << "Wins" 
             << "Win Rate (%)" << endl;
        cout << string(65, '-') << endl;

        for (size_t i = 0; i < records.size(); ++i) {
            cout << left 
                 << setw(10) << (i + 1)
                 << setw(20) << records[i].getName()
                 << setw(15) << records[i].getGamesPlayed()
                 << setw(10) << records[i].getWins()
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
    UserManager userManager;

    void displayCurrentTime() {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        cout << "Current Time: " << ctime(&currentTime);
    }

    string authenticatePlayer() {
    string username, password;
    int passwordAttempts = 0;
    int securityAnswerAttempts = 0;
    
    while (true) {
        cout << "Enter username: ";
        cin >> username;
        
        if (!userManager.userExists(username)) {
            cout << "New user. Please register.\n";
            cout << "Enter password: ";
            cin.ignore();
            password = getHiddenInput();
            
            cout << "Enter favorite color (security question): ";
            string color;
            cin >> color;
            cout << "Enter favorite sport (security question): ";
            string sport;
            cin >> sport;
            
            if (userManager.registerUser(username, password, color, sport)) {
                cout << "Registration successful!\n";
                return username;
            }
        } else {
            while (passwordAttempts < 3) {
                cout << "Enter password: ";
                if (passwordAttempts == 0) cin.ignore();
                password = getHiddenInput();
                
                if (userManager.authenticateUser(username, password)) {
                    return username;
                } else {
                    passwordAttempts++;
                    if (passwordAttempts >= 3) {
                        cout << "\nToo many incorrect password attempts.\n";
                        cout << "Proceeding to security verification...\n\n";
                        break;
                    }
                    cout << "Invalid password.\n";
                    cout << "1. Try again\n2. Reset password\nChoice (1-2): ";
                    int choice;
                    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
                        cout << "Invalid input. Please enter 1 or 2: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    
                    if (choice == 2) {
                        break;
                    }
                }
            }

            // Security questions process
            while (securityAnswerAttempts < 3) {
                string color, sport;
    
                cout << "Enter your favorite color: ";
                cin >> color;
    
            if (!userManager.verifyColor(username, color)) {
                securityAnswerAttempts++;
                cout << "Incorrect favorite color.\n";
            if (securityAnswerAttempts >= 3) break;
            continue;
        }
    
    cout << "Enter your favorite sport: ";
    cin >> sport;
    
    if (!userManager.verifySport(username, sport)) {
        securityAnswerAttempts++;
        cout << "Incorrect favorite sport.\n";
        if (securityAnswerAttempts >= 3) break;
        continue;
    }
    
    cout << "Enter new password: ";
    cin.ignore();
    string newPassword = getHiddenInput();
    userManager.resetPassword(username, color, sport, newPassword);
    cout << "Password reset successful!\n";
    return username;
}

            if (securityAnswerAttempts >= 3) {
                cout << "\nToo many incorrect security answer attempts.\n";
                cout << "As mentioned earlier, account recovery is not possible.\n";
                cout << "Please create a new account.\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                system("cls");
                continue;
            }
        }
    }
}
public:
    Menu() : isRunning(true) {
        leaderboard.loadLeaderboard();
    }

    bool isNameTaken(const string& name) {
        return find_if(players.begin(), players.end(),
            [&name](const Player& p) { return p.getName() == name; }) != players.end();
    }

    void handleStartGame() {
    system("cls");
    displayCurrentTime();
    cout << "\n=== Start New Game ===" << endl;
    
    string roomName;
    cout << "Enter room name (max 8 characters): ";
    cin.ignore();
    getline(cin, roomName);
    
    if (roomName.length() > 8) {
        roomName = roomName.substr(0, 8);
        cout << "Room name truncated to: " << roomName << endl;
    }
    
    game.setRoomName(roomName);
    
    cout << "\n!!! IMPORTANT ACCOUNT SECURITY WARNING !!!" << endl;
    cout << "Please remember your username, password, and security answers carefully." << endl;
    cout << "If you forget them, you won't be able to access your account again." << endl;
    cout << "There is no way to recover a lost account." << endl;
    cout << "\nPress Enter to continue...";
    cin.get();

    vector<string> playerNames;
    
    // Player 1 Authentication
    system("cls");
    displayCurrentTime();
    cout << "\n=== Player 1 Authentication ===" << endl;
    cout << string(30, '=') << endl;
    
    while (true) {
        string username = authenticatePlayer();
        playerNames.push_back(username);
        if (!isNameTaken(username)) {
            players.push_back(Player(username));
        }
        break;
    }
    
    cout << "\nPlayer 1 authentication successful!" << endl;
    cout << "Press Enter to continue to Player 2..." << endl;
    cin.get();
    
    // Player 2 Authentication
    system("cls");
    displayCurrentTime();
    cout << "\n=== Player 2 Authentication ===" << endl;
    cout << string(30, '=') << endl;
    
    while (true) {
        string username = authenticatePlayer();
        
        // Check if this username is already taken in current game
        if (username == playerNames[0]) {
            cout << "This user is already in the game. Please use a different account.\n";
            continue;
        }
        
        playerNames.push_back(username);
        if (!isNameTaken(username)) {
            players.push_back(Player(username));
        }
        break;
    }

    system("cls");
    displayCurrentTime();
    cout << "\n=== Game Starting ===" << endl;
    cout << string(30, '=') << endl;
    cout << "Both players authenticated successfully!\n\n";
    cout << "Player 1: " << playerNames[0] << endl;
    cout << "Player 2: " << playerNames[1] << endl;
    cout << "\nPress Enter to start the game...";
    cin.get();
    
    game.startGame(playerNames);
    
    while (!game.isGameOver()) {
        game.playTurn();
    }
    
    leaderboard.updateLeaderboard(game.getPlayers());
    leaderboard.saveLeaderboard();
    
    cout << "\nPress Enter to continue...";
    cin.get();
}

void handleLoadGame() {
    system("cls");
    displayCurrentTime();
    cout << "\n=== Load Game ===" << endl;
    
    Game::displaySavedGames();
    
    if (Game::roomManager.getActiveRooms().empty()) {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    cout << "\nEnter Number shown above to load (0 to cancel): ";
    int choice;
    cin >> choice;
    
    if (choice == 0) {
        return;
    }
    
    if (Game::loadGameByIndex(game, choice)) {
        cout << "\nGame loaded successfully!\n";
        cout << "Room Name: " << game.getRoomName() << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();

        while (!game.isGameOver()) {
            game.playTurn();
        }
        
        const vector<Player>& gamePlayers = game.getPlayers();
        for (const auto& player : gamePlayers) {
            auto it = find_if(players.begin(), players.end(),
                [&](const Player& p) { return p.getName() == player.getName(); });
            if (it == players.end()) {
                players.push_back(player);
            }
        }
        
        leaderboard.updateLeaderboard(players);
        leaderboard.saveLeaderboard();
    } else {
        cout << "Invalid room number.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void handleSearchRecord() {
    system("cls");
    cout << "\n=== Search Player Record ===" << endl;
    
    string searchName;
    cout << "Enter player name to search: ";
    cin >> searchName;

    auto leaderboardPlayer = find_if(leaderboard.records.begin(), leaderboard.records.end(),
        [&searchName](const Player& p) { return p.getName() == searchName; });

    if (leaderboardPlayer != leaderboard.records.end()) {
        cout << "\nBingo Game Statistics for " << searchName << ":\n";
        cout << string(40, '-') << "\n";
        cout << "Total Games Completed: " << leaderboardPlayer->getGamesPlayed() << "\n";
        cout << "Victories: " << leaderboardPlayer->getWins() << "\n";
        cout << "Win Rate: " << fixed << setprecision(1) << leaderboardPlayer->getWinRate() << "%\n";
        if(leaderboardPlayer->getGamesPlayed() > 0) {
            cout << "Losses: " << leaderboardPlayer->getGamesPlayed() - leaderboardPlayer->getWins() << "\n";
        }
    } else {
        cout << "\nNo records found for player: " << searchName << "\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}
    void handleViewLeaderboard() {
        system("cls");
        leaderboard.displayLeaderboard();
        cout << "\nPress Enter to continue...";
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
            displayCurrentTime();
            
            cout << "\n=== Bingo Game Menu ===" << endl;
            cout << "1. View Rules" << endl;
            cout << "2. Start Game" << endl;
            cout << "3. Load Saved Game" << endl;
            cout << "4. Search Record" << endl;
            cout << "5. View Leaderboard" << endl;
            cout << "6. Exit" << endl;
            cout << "Choose (1-6): ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: displayRules(); break;
                case 2: handleStartGame(); break;
                case 3: handleLoadGame(); break;
                case 4: handleSearchRecord(); break;
                case 5: handleViewLeaderboard(); break;
                case 6: exitProgram(); break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    system("pause");
                    break;
            }
        }
    }
};

RoomManager Game::roomManager;

int main() {
    Menu menu;
    menu.displayMainMenu();
    return 0;
} 