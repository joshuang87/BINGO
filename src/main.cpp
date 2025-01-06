#include "../include/server.h"
#include "../include/client.h"
#include <iostream>
using namespace std;

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