// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// File: main.cpp
// Description: Entry point — main menu, game loops for both
//              Basic Tic Tac Toe and Ultimate Tic Tac Toe
// ============================================================

#include "../include/ticTacToe.h"
#include "../include/ultimateTicTacToe.h"
#include <iostream>
#include <limits>
using namespace std;

// ----------------------------------------------------------
// Game loop for Problem 01: Basic Tic Tac Toe
// ----------------------------------------------------------
void playBasicTicTacToe() {
    cout << "\n========================================\n";
    cout << "        BASIC TIC TAC TOE\n";
    cout << "========================================\n";
    cout << "1. Start New Game\n";
    cout << "2. Load Saved Game\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    int boardSize = 3;
    ticTacToe* game = nullptr;

    if (choice == 2) {
        game = new ticTacToe(); // Temporary default board to load into
        if (!game->loadGame()) {
            cout << "No saved game found. Starting a new game instead.\n";
            delete game;
            game    = nullptr;
            choice  = 1;
        }
    }

    if (choice == 1 || game == nullptr) {
        cout << "Enter board size N (N >= 3): ";
        cin >> boardSize;
        while (boardSize < 3) {
            cout << "Invalid! N must be >= 3. Enter again: ";
            cin >> boardSize;
        }
        game = new ticTacToe(boardSize);
    }

    cout << "\nGame started! Enter row and column numbers (1-indexed).\n";
    cout << "Type '0 0' at any time to save and exit.\n\n";

    while (!game->isGameOver()) {
        cout << *game;
        cout << "Player " << game->getCurrentPlayer() << "'s turn.\n";
        cout << "Enter row and column (or 0 0 to save & exit): ";

        int row, col;
        cin >> row >> col;

        // Save and exit
        if (row == 0 && col == 0) {
            game->saveGame();
            delete game;
            return;
        }

        // Convert from 1-indexed (user) to 0-indexed (internal)
        if (!game->makeMove(row - 1, col - 1))
            cout << "Invalid move! Cell is occupied or out of bounds. Try again.\n";
    }

    // Display final board and result
    cout << *game;
    if (game->getWinner() == 'D')
        cout << "\n*** It's a DRAW! Well played! ***\n";
    else
        cout << "\n*** Player " << game->getWinner() << " WINS! Congratulations! ***\n";

    delete game;
}

// ----------------------------------------------------------
// Game loop for Problem 02: Ultimate Tic Tac Toe
// ----------------------------------------------------------
void playUltimateTicTacToe() {
    cout << "\n========================================\n";
    cout << "      ULTIMATE TIC TAC TOE\n";
    cout << "========================================\n";
    cout << "1. Start New Game\n";
    cout << "2. Load Saved Game\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    ultimateTicTacToe* game = new ultimateTicTacToe();

    if (choice == 2) {
        if (!game->loadGame())
            cout << "No saved game found. Starting a new game instead.\n";
    }

    cout << "\nUltimate Tic Tac Toe started!\n";
    cout << "Input format: BigRow BigCol SmallRow SmallCol (all 1-indexed)\n";
    cout << "Type '0 0 0 0' to save and exit.\n\n";

    while (!game->isGameOver()) {
        cout << *game;

        // Show board constraint from last move
        int nbr = game->getNextBoardRow();
        int nbc = game->getNextBoardCol();

        if (nbr != -1 && nbc != -1)
            cout << "Player " << game->getCurrentPlayer()
                 << " MUST play in board (" << nbr + 1 << ", " << nbc + 1 << ")\n";
        else
            cout << "Player " << game->getCurrentPlayer()
                 << " may play in ANY available board.\n";

        cout << "Enter BigRow BigCol SmallRow SmallCol (or 0 0 0 0 to save & exit): ";

        int br, bc, sr, sc;
        cin >> br >> bc >> sr >> sc;

        // Save and exit
        if (br == 0 && bc == 0 && sr == 0 && sc == 0) {
            game->saveGame();
            delete game;
            return;
        }

        // Convert to 0-indexed
        if (!game->makeMove(br - 1, bc - 1, sr - 1, sc - 1))
            cout << "Invalid move! Check board constraint and try again.\n";
    }

    // Display final result
    cout << *game;
    if (game->getWinner() == 'D')
        cout << "\n*** It's a DRAW! What a match! ***\n";
    else
        cout << "\n*** Player " << game->getWinner() << " WINS the Ultimate game! ***\n";

    delete game;
}

// ----------------------------------------------------------
// Main: display main menu and route to game functions
// ----------------------------------------------------------
int main() {
    cout << "========================================\n";
    cout << "   CS112 OOP - Assignment 02\n";
    cout << "   Tic Tac Toe Games\n";
    cout << "   Air University, Islamabad\n";
    cout << "========================================\n";

    int choice;
    do {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Play Basic Tic Tac Toe   (Problem 01)\n";
        cout << "2. Play Ultimate Tic Tac Toe (Problem 02)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // Handle invalid (non-integer) input gracefully
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: playBasicTicTacToe();    break;
            case 2: playUltimateTicTacToe(); break;
            case 3: cout << "Goodbye!\n";   break;
            default: cout << "Invalid choice. Enter 1, 2, or 3.\n";
        }
    } while (choice != 3);

    return 0;
}
