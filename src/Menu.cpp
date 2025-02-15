/**
 * @file Menu.cpp
 * @brief Implementation of the Menu class functionality
 */

#include "../include/Menu.h"
#include "../include/Leaderboard.h"
#include "../include/Game.h"
#include "../include/DB.h"
#include "../include/Util.h"

#include <chrono>

/**
 * @brief Constructor initializes the menu with isRunning set to true
 */
Menu::Menu() : isRunning(true) {}

/**
 * @brief Displays the game rules in a formatted manner
 * 
 * Shows a list of rules including:
 * - Board size and number range
 * - Turn-based gameplay
 * - Marking mechanism
 * - Win conditions
 * - Board visibility rules
 * - Input instructions
 */
void Menu::displayRules() {
    cout << "\n=== Bingo Rules ===" << endl;  // simplified title
    cout << "1. You will get a 5x5 board with numbers 1-25" << endl;
    cout << "2. Take turns picking numbers" << endl;
    cout << "3. Numbers get marked with X on boards" << endl;
    cout << "4. Get 5 lines to win (rows/columns/diagonals)" << endl;
    cout << "5. Can't see other player's board" << endl;
    cout << "6. Enter 1-25 to play or Q to quit" << endl;
    
    cout << "\nHit Enter to back game menu...";
    cin.ignore();
    cin.get();
}

/**
 * @brief Displays the current system time
 * 
 * Uses chrono to get and display the current system time
 * in a human-readable format.
 */
void Menu::displayCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    cout << "Current Time: " << ctime(&currentTime);
}

/**
 * @brief Handles the process of starting a new game
 * @param p1 Reference to Player 1
 * @param p2 Reference to Player 2
 * 
 * This method:
 * 1. Creates a new game instance
 * 2. Displays player information
 * 3. Manages the game loop until completion
 * 4. Updates player statistics after the game
 */
void Menu::handleStartGame(Player& p1, Player& p2) {
    system("cls");
    displayCurrentTime();
    
    Game game = Game();

    system("cls");
    displayCurrentTime();
    cout << "\n=== Game Starting ===" << endl;
    cout << string(30, '=') << endl;
    cout << "Player 1: " << p1.getUsername() << endl;
    cout << "Player 2: " << p2.getUsername() << endl;
    cout << "\nPress Enter to start the game...";
    cin.get();
        
    vector<Player*> players = {&p1, &p2};
    game.startGame(players);
        
    while (!game.isGameOver()) {
        game.playTurn();
    }

    // reload player saved data
    vector<Player> updatedPlayers = DB::getInstance().load<Player>();
    for (const Player& updatedPlayer : updatedPlayers) {
        if (updatedPlayer.getUsername() == p1.getUsername()) {
            p1 = updatedPlayer;
        }
        if (updatedPlayer.getUsername() == p2.getUsername()) {
            p2 = updatedPlayer;
        }
    }
        
    cout << "\nPress Enter to continue...";
    cin.get();
}

/**
 * @brief Handles loading a saved game
 * @param p1 Reference to Player 1
 * @param p2 Reference to Player 2
 * 
 * This method:
 * 1. Loads saved games from the database
 * 2. Displays available saved games
 * 3. Processes user input for game selection
 * 4. Loads and continues the selected game
 */
void Menu::handleLoadGame(Player& p1, Player& p2) {
    system("cls");
    displayCurrentTime();
    cout << "\n=== Load Game ===" << endl;
    
    vector<Game> games = DB::getInstance().load<Game>();
    
    if (!Game::displaySavedGames(games, p1, p2)) {
        return;
    }
    
    cout << "\nEnter Number shown above to load (0 or other to cancel): ";
    string input;
    cin >> input;
    
    if (input.find_first_not_of("0123456789") != string::npos) {
        cout << "Invalid input. Please enter a number from the list above.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    try {
        int choice = stoi(input);
        
        //if 0 then back to main menu
        if (choice == 0) {
            return;
        }
        
        //check num
        if (choice < 1 || choice > static_cast<int>(games.size())) {
            cout << "Please enter a number between 1 and " << games.size() << ".\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        Game game = games[choice - 1];
        vector<Player> ps = {p1, p2};
        game.loadPlayerData(ps);
        game.continueGame();

    } catch (...) {
        cout << "Invalid input. Please enter a number from the list above.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }    
}

/**
 * @brief Handles searching and displaying player records
 * 
 * This method:
 * 1. Loads all players from the database
 * 2. Displays a list of players
 * 3. Allows user to select a player
 * 4. Shows detailed statistics for the selected player
 */
void Menu::handleSearchRecord() {
    vector<Player> players = DB::getInstance().load<Player>();
    if (players.empty()) {
        cout << "No players found." << endl;
        Util::waitEnter();
        return;
    }

    do {
        system("cls");
        cout << "\n=== Search Player Record ===" << endl;

        int count = 0;
        string choice;
        for (Player& player : players) {
            count++;
            cout << count << ". " << player.getUsername() << endl;
        }

        Util::showLine();
        cout << "Enter Player Number: ";
        cin >> choice;

        if (!Util::isNumber(choice)) {
            cout << "Invalid input! Please enter again.\n";
            Util::showLine();
            Util::waitEnter();
            continue;
        }

        int input = stoi(choice);
        if (input < 1 || input > players.size()) {
            cout << "Invalid input! Please enter again.\n";
            Util::showLine();
            Util::waitEnter();
            continue;
        }

        cout << "\nBingo Game Statistics for " << players[input - 1].getUsername() << ":\n";
        cout << string(40, '-') << "\n";
        cout << "Total Games Completed: " << players[input - 1].getGameCount() << "\n";
        cout << "Victories: " << players[input - 1].getWinCount() << "\n";
        cout << "Losses: " << players[input - 1].getLoseCount() << "\n";
        cout << "Win Rate: " << fixed << setprecision(1) << players[input - 1].getWinRate() << "%\n";
        Util::showLine();
        Util::waitEnter();
        break;
    } while (true);
}

/**
 * @brief Handles displaying the leaderboard
 * 
 * Clears the screen, displays the leaderboard, and waits for user input
 * before returning to the main menu.
 */
void Menu::handleViewLeaderboard() {
    system("cls");
    leaderboard.displayLeaderboard();
    Util::showLine();
    Util::waitEnter();
}

/**
 * @brief Handles program exit
 * 
 * Sets isRunning to false and displays a goodbye message.
 */
void Menu::exitProgram() {
    isRunning = false;
    cout << "\nThank you for playing! Goodbye!\n";
}

/**
 * @brief Displays and handles the main menu
 * @param p1 Reference to Player 1
 * @param p2 Reference to Player 2
 * 
 * This method:
 * 1. Displays menu options in a loop
 * 2. Processes user input
 * 3. Calls appropriate handler methods based on user choice
 * 4. Continues until the program is exited
 */
void Menu::displayMainMenu(Player& p1, Player& p2) {
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

        string input;
        cin >> input;
            
        if (input.length() == 1 && input[0] >= '1' && input[0] <= '6') {
            int choice = input[0] - '0';
                
            switch (choice) {
                case 1: displayRules(); break;
                case 2: handleStartGame(p1, p2); break;
                case 3: handleLoadGame(p1, p2); break;
                case 4: handleSearchRecord(); break;
                case 5: handleViewLeaderboard(); break;
                case 6: exitProgram(); break;
            }
        } else {
            cout << "Please enter a number between 1 and 6." << endl;
            cout << "Press Enter to continue..." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}