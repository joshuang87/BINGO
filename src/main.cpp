/**
 * @file main.cpp
 * @brief Main entry point for the BINGO game application
 * 
 * This file contains the main function that:
 * 1. Initializes the logging system and database
 * 2. Handles player authentication for both players
 * 3. Launches the main game menu
 */

#include "../include/DB.h"
#include "../include/Logger.h"
#include "../include/Account.h"
#include "../include/Player.h"
#include "../include/Menu.h"
#include "../include/Util.h"

/**
 * @brief Main entry point of the BINGO game
 * 
 * The function performs the following steps:
 * 1. Initializes the logging system with "app.log" as the log file
 * 2. Initializes the database connection
 * 3. Authenticates Player 1 through login/signup
 * 4. Authenticates Player 2, ensuring a different account from Player 1
 * 5. Creates and displays the main game menu
 * 
 * @return int Returns 0 on successful execution
 */
int main() {
    // Initialize logging and database systems
    Logger::getInstance().init("app.log");
    DB::getInstance().init();

    Player *player1, *player2;

    // Authenticate Player 1
    cout << "===== BINGO =====" << endl;
    cout << "Player 1 : " << endl;
    player1 = Player::authenticator();
    cout << "Player 1 : " << player1->getUsername() << " is ready!!"<< endl;
    Util::waitEnter();

    // Authenticate Player 2 (must be different from Player 1)
    do {
        cout << "Player 2 : " << endl;
        player2 = Player::authenticator();
        if (player2->getUsername() == player1->getUsername()) {
            cout << "This account already signed in as Player 1. Please sign in with another account." << endl;
            Util::waitEnter();
            continue;
        }
        else {
            cout << "Player 2 : " << player2->getUsername() << " is ready!!"<< endl;
        }
        Util::waitEnter();
        break;
    } while (true);

    // Create and display the main game menu
    Menu menu;
    menu.displayMainMenu(*player1, *player2);
    
    return 0;
}