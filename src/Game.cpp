#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/DB.h"
#include "../include/Util.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <filesystem>

using namespace std;

Game::Game(bool empty) : currentTurn(0), isOver(false), winner(nullptr) {
    if (!empty) {
        gameId = generateGameId();
    }
}

#pragma region Getters

string Game::getGameId() const {
    return gameId;
}

Player* Game::getWinner() const { 
    return winner; 
}

const vector<Player>& Game::getPlayers() const { 
    return players; 
}

string Game::getCurrentTurn() const { 
    return to_string(currentTurn); 
}

#pragma endregion

#pragma region Setters

void Game::setGameId(const string& gameId) {
    this->gameId = gameId;
}

void Game::setWinner(Player* player) { 
    winner = player; 
}   

#pragma endregion
 
bool Game::isGameOver() const { 
    return isOver; 
}

void Game::save() {
    // Read existing game data
    ifstream inFile("../data/Game.json");
    string jsonContent;
    string line;
    while (getline(inFile, line)) {
        jsonContent += line;
    }
    inFile.close();

    // Parse existing games
    vector<string> existingGames;
    size_t start = jsonContent.find('[');
    size_t end = jsonContent.find_last_of(']');
    if (start != string::npos && end != string::npos) {
        string gamesStr = jsonContent.substr(start + 1, end - start - 1);
        size_t pos = 0;
        int braceCount = 0;
        string currentGame;
        
        for (size_t i = 0; i < gamesStr.length(); ++i) {
            char c = gamesStr[i];
            if (c == '{') {
                braceCount++;
                currentGame += c;
            }
            else if (c == '}') {
                braceCount--;
                currentGame += c;
                if (braceCount == 0 && !currentGame.empty()) {
                    // 去除前导和尾随空格
                    while (!currentGame.empty() && isspace(currentGame.front())) {
                        currentGame.erase(0, 1);
                    }
                    while (!currentGame.empty() && isspace(currentGame.back())) {
                        currentGame.pop_back();
                    }
                    if (!currentGame.empty()) {
                        existingGames.push_back(currentGame);
                    }
                    currentGame.clear();
                }
            }
            else if (braceCount > 0) {
                currentGame += c;
            }
        }
    }

    // Create new game state JSON
    string newGameState = "{";
    newGameState += "\"ID\":\"" + getGameId() + "\",";
    
    // Add players array
    newGameState += "\"Players\":[";
    for (size_t i = 0; i < players.size(); ++i) {
        newGameState += "\"" + players[i].getUsername() + "\"";
        if (i < players.size() - 1) newGameState += ",";
    }
    newGameState += "],";

    // Add Current turn player index
    newGameState += "\"CurrentTurn\":" + to_string(currentTurn) + ",";

    // Add winner
    newGameState += "\"Winner\":\"" + (winner ? winner->getUsername() : "") + "\",";

    // Add game status
    newGameState += "\"Status\":[{";
    for (size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        if (i > 0) newGameState += ",";
        newGameState += "\"" + player.getUsername() + "\":" + player.getBoardState();
    }
    newGameState += "}]}";

    // Update or add the game state
    bool roomFound = false;
    string finalJson = "[";
    for (size_t i = 0; i < existingGames.size(); ++i) {
        if (!existingGames[i].empty()) {  // 只处理非空的游戏数据
            if (existingGames[i].find("\"ID\":\"" + getGameId() + "\"") != string::npos) {
                if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
                finalJson += newGameState;
                roomFound = true;
            } else {
                if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
                finalJson += existingGames[i];
            }
        }
    }
    
    if (!roomFound) {
        if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
        finalJson += newGameState;
    }
    finalJson += "]";

    // Write back to file
    ofstream outFile("../data/Game.json");
    if (outFile.is_open()) {
        outFile << finalJson;
        outFile.close();
        cout << "Game saved successfully in room: " << getGameId() << "\n";
    } else {
        cout << "Error: Could not save game state.\n";
    }
}

void Game::save(vector<Game>& games) {
    // Read existing game data
    ifstream inFile("../data/Game.json");
    string jsonContent;
    string line;
    while (getline(inFile, line)) {
        jsonContent += line;
    }
    inFile.close();

    // Parse existing games
    vector<string> existingGames;
    size_t start = jsonContent.find('[');
    size_t end = jsonContent.find_last_of(']');
    if (start != string::npos && end != string::npos) {
        string gamesStr = jsonContent.substr(start + 1, end - start - 1);
        size_t pos = 0;
        int braceCount = 0;
        string currentGame;
        
        for (size_t i = 0; i < gamesStr.length(); ++i) {
            char c = gamesStr[i];
            if (c == '{') {
                braceCount++;
                currentGame += c;
            }
            else if (c == '}') {
                braceCount--;
                currentGame += c;
                if (braceCount == 0 && !currentGame.empty()) {
                    // 去除前导和尾随空格
                    while (!currentGame.empty() && isspace(currentGame.front())) {
                        currentGame.erase(0, 1);
                    }
                    while (!currentGame.empty() && isspace(currentGame.back())) {
                        currentGame.pop_back();
                    }
                    if (!currentGame.empty()) {
                        existingGames.push_back(currentGame);
                    }
                    currentGame.clear();
                }
            }
            else if (braceCount > 0) {
                currentGame += c;
            }
        }
    }

    // Create new game state JSON
    string newGameState;

    for (Game& game : games) {
        if (games.begin()->generateGameId() == game.getGameId()) {
            newGameState += "{";
        }
        else{
            newGameState += ",{";
        }
        newGameState += "\"ID\":\"" + game.getGameId() + "\",";
        
        // Add players array
        newGameState += "\"Players\":[";
        for (size_t i = 0; i < game.getPlayers().size(); ++i) {
            newGameState += "\"" + game.getPlayers()[i].getUsername() + "\"";
            if (i < game.getPlayers().size() - 1) newGameState += ",";
        }

        newGameState += "],";

        // Add Current turn player index
        newGameState += "\"CurrentTurn\":" + game.getCurrentTurn() + ",";

        // Add winner
        newGameState += "\"Winner\":\"" + (game.getWinner() ? game.getWinner()->getUsername() : "") + "\",";

        // Add game status
        newGameState += "\"Status\":[{";
        for (size_t i = 0; i < players.size(); ++i) {
            const auto& player = players[i];
            if (i > 0) newGameState += ",";
            newGameState += "\"" + player.getUsername() + "\":" + player.getBoardState();
        }
        newGameState += "}]}";
    }

    // Update or add the game state
    bool roomFound = false;
    string finalJson = "[";
    for (size_t i = 0; i < existingGames.size(); ++i) {
        if (!existingGames[i].empty()) {  // 只处理非空的游戏数据
            if (existingGames[i].find("\"ID\":\"" + getGameId() + "\"") != string::npos) {
                if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
                finalJson += newGameState;
                roomFound = true;
            } else {
                if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
                finalJson += existingGames[i];
            }
        }
    }
    
    if (!roomFound) {
        if (finalJson.length() > 1) finalJson += ",";  // 只在需要时添加逗号
        finalJson += newGameState;
    }
    finalJson += "]";

    // Write back to file
    ofstream outFile("../data/Game.json");
    if (outFile.is_open()) {
        outFile << finalJson;
        outFile.close();
        // cout << "Game saved successfully in room: " << getGameId() << "\n";
    } else {
        cout << "Error: Could not save game state.\n";
    }
}

bool Game::isGameSaved() const {
    return isSaved;
}

void Game::cleanupRoom() {
    if (!isOver) {
        char saveChoice;
        cout << "Do you want to save the game state? (Y/N): ";
        cin >> saveChoice;
        if (toupper(saveChoice) == 'Y') {
            save();
            isSaved = true;
            cout << "Game saved successfully in room: " << getGameId() << "\n";
        }
    }
}

bool Game::displaySavedGames(vector<Game>& allSavedGames, Player& p1, Player& p2) {
    if (allSavedGames.empty()) {
        cout << "\nNo saved games found.\n";
        Util::waitEnter();
        return false;
    }

    vector<Game> savedGames;

    for (Game& game : allSavedGames) {
        if (game.getPlayers()[0].getUsername() == p1.getUsername() && game.getPlayers()[1].getUsername() == p2.getUsername()) {
            savedGames.push_back(game);
        }
    }

    if (savedGames.empty()) {
        cout << "\nNo saved games found.\n";
        Util::waitEnter();
        return false;
    }

    cout << "\nAvailable saved games:\n";
    cout << string(50, '=') << endl;
    cout << left << setw(10) << "Number" << setw(20) << "Game ID" << endl;
    cout << string(50, '-') << endl;
    
    for (size_t i = 0; i < savedGames.size(); ++i) {
        cout << left << setw(10) << (i + 1) << setw(20) << savedGames[i].getGameId() << endl;
    }
    cout << string(50, '=') << endl;
    cout << "\nPlease enter the Number (1-" << savedGames.size() << ") to select a game." << endl;
    return true;
}

void Game::startGame(vector<Player*> ps) {

    for (const Player* p : ps) {
        players.push_back(*p);
        players.back().generateBoard();
    }

    currentTurn = 0;
    isOver = false;

    cout << "Game started between " << players[0].getUsername()
        << " and " << players[1].getUsername() << ".\n\n";

    cout << players[currentTurn].getUsername() << "'s board:\n";
    players[currentTurn].displayBoard();
}

void Game::playTurn() {
    if (isOver) {
        cout << "Game is already over.\n";
        return;
    }

    Player& currentPlayer = players[currentTurn];
    system("cls");

    cout << currentPlayer.getUsername() << "'s turn.\n";
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
    cout << currentPlayer.getUsername() << "'s board after marking " << number << ":\n";
    currentPlayer.displayBoard();

    bool gameWon = false;

    for (auto& player : players) {
        if (player.checkWin()) {
            gameWon = true;
            setWinner(&player);
            break;
        }
    }

    if (gameWon && getWinner() != nullptr) {
        cout << "\n" << getWinner()->getUsername() << " wins!\n";
        isOver = true;

        for (auto& player : players) {
            player.updateStats(player.getUsername() == getWinner()->getUsername());
        }

        vector<Player> allSavedPlayers = DB::getInstance().load<Player>();

        allSavedPlayers.erase(
            remove_if(allSavedPlayers.begin(), allSavedPlayers.end(),
                [this](const Player& savedPlayer) {
                    return any_of(players.begin(), players.end(),
                        [&savedPlayer](const Player& currentPlayer) {
                            return savedPlayer.getUsername() == currentPlayer.getUsername();
                        });
                }),
            allSavedPlayers.end()
        );

        cout << "\nFinal boards:\n";
        for (const auto& player : players) {
            allSavedPlayers.push_back(player);
            cout << "\n" << player.getUsername() << "'s board:\n";
            player.displayBoard();
        }
        
        DB::getInstance().reset<Player>();
        for (Player& player : allSavedPlayers) {
            DB::getInstance().save(player);
        }

        vector<Game> savedGames = DB::getInstance().load<Game>();

        savedGames.erase(
            remove_if(savedGames.begin(), savedGames.end(),
                [this](const Game& game) {
                    return game.getGameId() == getGameId();
                }),
            savedGames.end()
        );
        DB::getInstance().reset<Game>();
        Game::save(savedGames);
        cout << "Player data updated...";
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
    cout << players[currentTurn].getUsername() << "'s turn.\n";
    cout << "Your board:\n";
    players[currentTurn].displayBoard();
}

vector<vector<int>> Game::parseGameStatusData(const string& json) {
    vector<vector<int>> result;
    vector<int> currentRow;
    
    // 跳过前缀
    size_t pos = json.find("[[");
    if (pos == string::npos) return result;
    pos += 2;
    
    while (pos < json.length()) {
        if (json[pos] == '[') {
            // 新的一行开始
            currentRow.clear();
        }
        else if (json[pos] == ']') {
            // 一行结束
            if (!currentRow.empty()) {
                result.push_back(currentRow);
                currentRow.clear();
            }

            // 检查是否到达最后的"]]"
            if (pos + 1 < json.length() && json[pos + 1] == ']') {
                break;  // 遇到"]]"就直接结束
            }
        }
        else if (json[pos] == '\"') {
            // 读取数字或 'x'
            size_t nextQuote = json.find('\"', pos + 1);
            if (nextQuote == string::npos) break;
            
            string value = json.substr(pos + 1, nextQuote - pos - 1);
            if (value == "x") {
                currentRow.push_back(-1); // 用-1表示'x'
            } else {
                try {
                    currentRow.push_back(std::stoi(value));
                } catch (...) {
                    // 处理转换错误
                    currentRow.push_back(-1);
                }
            }
            pos = nextQuote;
        }
        pos++;
    }
    
    return result;
}

vector<vector<bool>> Game::parseGameMarkedData(const vector<vector<int>>& data) {
    vector<vector<bool>> result;
    
    // 对每一行进行处理
    for (const auto& row : data) {
        vector<bool> boolRow;
        // 检查每个元素
        for (int val : row) {
            boolRow.push_back(val == -1);  // 如果是-1(x)则为true，否则为false
        }
        result.push_back(boolRow);
    }
    
    return result;
}

vector<Game> Game::from_json(const string& json) {
    vector<Game> games;
    stringstream ss(json);
    string line;
    int loopCount = 0, gameIndex = 0;

    if (json == "[]" || json.empty()) {
        return games;
    }

    while (getline(ss, line, '{')) {
        // Skip empty entries
        if (line == "[") continue;
        Game game(true);
        size_t pos;

        if (loopCount % 2 != 0) {
            // Parse Status array for game boards
            size_t posP1 = line.find(games[gameIndex].getPlayers()[0].getUsername());
            size_t posP2 = line.find(games[gameIndex].getPlayers()[1].getUsername());
            string dataP1 = line.substr(posP1, posP2);
            string dataP2 = line.substr(posP2);

            vector<vector<int>> boardP1 = parseGameStatusData(dataP1);
            vector<vector<bool>> markedBoardP1 = parseGameMarkedData(boardP1);

            vector<vector<int>> boardP2 = parseGameStatusData(dataP2);
            vector<vector<bool>> markedBoardP2 = parseGameMarkedData(boardP2);

            games[gameIndex].players[0].setBoard(boardP1, markedBoardP1);
            games[gameIndex].players[1].setBoard(boardP2, markedBoardP2);

            gameIndex++;
        }

        if (loopCount % 2 == 0) {

            // Parse Room ID/Name
            pos = line.find("ID");
            string id = line.substr(pos + 5); // skip "ID":"
            game.setGameId(id.substr(0, id.find("\"")));

            vector<Player> players;
            // Parse Players array
            pos = line.find("Players");
            if (pos != string::npos) {
                string playersStr = line.substr(pos);
                pos = playersStr.find("[");
                playersStr = playersStr.substr(pos + 1);
                size_t endPos = playersStr.find("]");
                playersStr = playersStr.substr(0, endPos);

                // Split players string by comma
                stringstream playersSS(playersStr);
                string playerName;
                vector<Account> accList = DB::getInstance().load<Account>();
                
                while (getline(playersSS, playerName, ',')) {
                    // Remove quotes and whitespace
                    playerName = playerName.substr(playerName.find("\"") + 1);
                    playerName = playerName.substr(0, playerName.find("\""));
                    for (const Account& acc : accList) {
                        if (acc.getUsername() == playerName) {
                            Player player(acc.getUsername(), acc.getPassword());
                            players.push_back(player);
                        }
                    }
                }
            }
            game.players = players;

            // Parse Winner
            pos = line.find("Winner");
            if (pos != string::npos) {
                string winner = line.substr(pos + 9); // skip "Winner":"
                winner = winner.substr(0, winner.find("\""));
                if (!winner.empty()) {
                    // Find the winning player and set them as winner
                    for (Player& player : game.players) {
                        if (player.getUsername() == winner) {
                            game.winner = &player;
                            game.isOver = true;
                            break;
                        }
                    }
                }
            }

            // Parse CurrentTurn
            pos = line.find("CurrentTurn");
            if (pos != string::npos) {
                string turnStr = line.substr(pos + 12); // skip "CurrentTurn":
                size_t endPos = turnStr.find(",");
                if (endPos != string::npos) {
                    turnStr = turnStr.substr(1, endPos);
                    game.currentTurn = stoi(turnStr);
                }
            }

            games.push_back(game);
        }

        loopCount++;
    }

    return games;
}

bool Game::isGameIdExist(const string& gameId) {
    vector<Game> games = DB::getInstance().load<Game>();
    for (const Game& game : games) {
        if (game.getGameId() == gameId) return true;
    }
    return false;
}

string Game::generateGameId() {
    int nextId = 1;
    string gameId;
    do {
        gameId = "Game_" + to_string(nextId);
        nextId++;
    } while (isGameIdExist(gameId));
    
    return gameId;
}

void Game::continueGame() {
    while (!isGameOver()) {
        playTurn();
    }

    cout << "\nGame over. Press Enter to continue...";
    cin.get();
}