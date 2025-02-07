# BINGO Game

A C++ implementation of the classic BINGO game with modern features and multiplayer support.

## Features

- Player vs Player gameplay
- Save/Load game functionality
- Player statistics tracking
- Leaderboard system
- Account management
- Detailed game logging

## Project Structure

- `include/` - Header files
  - `Game.h` - Game logic and state management
  - `Player.h` - Player class and statistics
  - `Account.h` - User account management
  - `DB.h` - Data persistence
  - `Leaderboard.h` - Leaderboard functionality
  - `Logger.h` - Logging system
  - `Menu.h` - User interface menus
  - `Util.h` - Utility functions

- `src/` - Source files implementation
- `data/` - Game data storage
- `Log/` - Game logs

## Getting Started

### Prerequisites

- C++ compiler supporting C++17 or later
- File system support for data persistence

### Building the Project

1. Clone the repository
2. Navigate to the project directory
3. Compile the source files:
   ```bash
   g++ src/*.cpp -I include/ -o bingo
   ```

### Running the Game

Execute the compiled binary:
```bash
./bingo
```

## Gameplay

1. Create an account or log in
2. Choose to start a new game or continue a saved game
3. Take turns marking numbers on your BINGO board
4. First player to complete a valid BINGO pattern wins

## Game Features

- **Save/Load**: Save your game progress and continue later
- **Statistics**: Track your win/loss record and win rate
- **Leaderboard**: Compete for top rankings
- **Logging**: Detailed game logs for review

## File Structure

- Game states are saved in JSON format
- Player data is persistently stored
- Comprehensive logging system for debugging and game history

## Contributing

Feel free to fork the repository and submit pull requests for improvements.

## License

This project is created for educational purposes at Multimedia University (MMU).