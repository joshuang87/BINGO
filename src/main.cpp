#include "../include/DB.h"
#include "../include/Logger.h"
#include "../include/Account.h"
#include "../include/Player.h"
#include "../include/Menu.h"
#include "../include/Util.h"

int main() {
    Logger::getInstance().init("app.log");
    DB::getInstance().init();

    Player *player1, *player2;

    cout << "===== BINGO =====" << endl;
    cout << "Player 1 : " << endl;
    player1 = Player::authenticator();
    cout << "Player 1 : " << player1->getUsername() << " is ready!!"<< endl;
    Util::waitEnter();

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

    Menu menu;
    menu.displayMainMenu(*player1, *player2);
    
    return 0;
}