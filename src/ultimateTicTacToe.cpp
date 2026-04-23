// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// File: ultimateTicTacToe.cpp
// Description: Method definitions for the ultimateTicTacToe class
// ============================================================

#include "../include/ultimateTicTacToe.h"
#include <iostream>
#include <fstream>
using namespace std;

// ----------------------------------------------------------
// Helper: Convert (bigRow, bigCol) to flat array index 0-8
// ----------------------------------------------------------
int ultimateTicTacToe::boardIndex(int br, int bc) const {
    return br * 3 + bc;
}

// ----------------------------------------------------------
// Helper: Return true if the small board at (br, bc) is still playable
// ----------------------------------------------------------
bool ultimateTicTacToe::isBoardPlayable(int br, int bc) const {
    return metaBoard[br][bc] == '.';
}

// ----------------------------------------------------------
// 1. Constructor: initialize 9 small boards and 3x3 meta board
// ----------------------------------------------------------
ultimateTicTacToe::ultimateTicTacToe()
    : currentPlayer('X'), nextBoardRow(-1), nextBoardCol(-1), winner('\0')
{
    boards = new ticTacToe[9]; // Each defaults to 3x3 via ticTacToe default constructor

    // Allocate and initialize the 3x3 meta board
    metaBoard = new char*[3];
    for (int i = 0; i < 3; i++) {
        metaBoard[i] = new char[3];
        for (int j = 0; j < 3; j++)
            metaBoard[i][j] = '.';
    }
}

// ----------------------------------------------------------
// 2. Copy Constructor: deep copy all 9 boards and meta board
// ----------------------------------------------------------
ultimateTicTacToe::ultimateTicTacToe(const ultimateTicTacToe& other) {
    currentPlayer = other.currentPlayer;
    nextBoardRow  = other.nextBoardRow;
    nextBoardCol  = other.nextBoardCol;
    winner        = other.winner;

    // Deep copy 9 small boards using ticTacToe's copy assignment
    boards = new ticTacToe[9];
    for (int i = 0; i < 9; i++)
        boards[i] = other.boards[i];

    // Deep copy meta board
    metaBoard = new char*[3];
    for (int i = 0; i < 3; i++) {
        metaBoard[i] = new char[3];
        for (int j = 0; j < 3; j++)
            metaBoard[i][j] = other.metaBoard[i][j];
    }
}

// ----------------------------------------------------------
// 3. Destructor: deallocate all dynamic memory
// ----------------------------------------------------------
ultimateTicTacToe::~ultimateTicTacToe() {
    delete[] boards;          // Each ticTacToe destructor handles its own board
    boards = nullptr;

    for (int i = 0; i < 3; i++)
        delete[] metaBoard[i];
    delete[] metaBoard;
    metaBoard = nullptr;
}

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
char ultimateTicTacToe::getWinner()        const { return winner; }
char ultimateTicTacToe::getCurrentPlayer() const { return currentPlayer; }
int  ultimateTicTacToe::getNextBoardRow()  const { return nextBoardRow; }
int  ultimateTicTacToe::getNextBoardCol()  const { return nextBoardCol; }
bool ultimateTicTacToe::isGameOver()       const { return winner != '\0'; }

// ----------------------------------------------------------
// 6. Check if a move is valid in ultimate game context
// ----------------------------------------------------------
bool ultimateTicTacToe::isValidMove(int bigRow, int bigCol, int smallRow, int smallCol) const {
    // Bounds check for big board
    if (bigRow < 0 || bigRow >= 3 || bigCol < 0 || bigCol >= 3) return false;
    // Bounds check for small board cell
    if (smallRow < 0 || smallRow >= 3 || smallCol < 0 || smallCol >= 3) return false;

    // If constrained to a specific board, enforce it
    if (nextBoardRow != -1 && nextBoardCol != -1) {
        if (bigRow != nextBoardRow || bigCol != nextBoardCol) return false;
    }

    // Target small board must still be playable (not won or drawn)
    if (!isBoardPlayable(bigRow, bigCol)) return false;

    // Target cell inside the small board must be empty
    return boards[boardIndex(bigRow, bigCol)].isValidMove(smallRow, smallCol);
}

// ----------------------------------------------------------
// 5. Make a move: updates sub-board, meta board, and next board constraint
// ----------------------------------------------------------
bool ultimateTicTacToe::makeMove(int bigRow, int bigCol, int smallRow, int smallCol) {
    if (!isValidMove(bigRow, bigCol, smallRow, smallCol)) return false;

    int idx = boardIndex(bigRow, bigCol);

    // Place the move on the small board
    boards[idx].makeMove(smallRow, smallCol);

    // If the small board has been decided, record it on the meta board
    char smallWinner = boards[idx].getWinner();
    if (smallWinner != '\0') {
        metaBoard[bigRow][bigCol] = smallWinner; // 'X', 'O', or 'D'
    }

    // 7. Check if the ultimate game has been won
    if (checkUltimateWinner()) {
        winner = currentPlayer;
    } else if (allBoardsDone()) {
        winner = 'D'; // All boards resolved but no one won the meta board
    }

    // Switch to the other player
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

    // Determine the next forced board based on the cell played in
    if (isBoardPlayable(smallRow, smallCol)) {
        nextBoardRow = smallRow;
        nextBoardCol = smallCol;
    } else {
        // Target board already done; allow free choice
        nextBoardRow = -1;
        nextBoardCol = -1;
    }

    return true;
}

// ----------------------------------------------------------
// 7. Check if ultimate game has a winner on the meta board
// ----------------------------------------------------------
bool ultimateTicTacToe::checkUltimateWinner() const {
    char p = currentPlayer;

    // Check rows
    for (int i = 0; i < 3; i++) {
        if (metaBoard[i][0] == p && metaBoard[i][1] == p && metaBoard[i][2] == p)
            return true;
    }
    // Check columns
    for (int j = 0; j < 3; j++) {
        if (metaBoard[0][j] == p && metaBoard[1][j] == p && metaBoard[2][j] == p)
            return true;
    }
    // Check main diagonal
    if (metaBoard[0][0] == p && metaBoard[1][1] == p && metaBoard[2][2] == p) return true;
    // Check anti-diagonal
    if (metaBoard[0][2] == p && metaBoard[1][1] == p && metaBoard[2][0] == p) return true;

    return false;
}

// ----------------------------------------------------------
// Check if every small board has been resolved (won or drawn)
// ----------------------------------------------------------
bool ultimateTicTacToe::allBoardsDone() const {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (metaBoard[i][j] == '.') return false;
    return true;
}

// ----------------------------------------------------------
// 4. Stream Output Operator (cout): print all 9 boards + meta board
// ----------------------------------------------------------
ostream& operator<<(ostream& out, const ultimateTicTacToe& game) {
    out << "\n=== ULTIMATE TIC TAC TOE ===\n";

    // Print 3 big rows; each big row contains 3 small boards side by side
    for (int bigRow = 0; bigRow < 3; bigRow++) {

        // Column number headers for each sub-board
        out << "      ";
        for (int bigCol = 0; bigCol < 3; bigCol++) {
            out << "  1  2  3  ";
            if (bigCol < 2) out << "  |  ";
        }
        out << "\n";

        // Print 3 rows of cells across each sub-board
        for (int smallRow = 0; smallRow < 3; smallRow++) {
            out << "      ";
            for (int bigCol = 0; bigCol < 3; bigCol++) {
                int idx = game.boardIndex(bigRow, bigCol);

                if (game.metaBoard[bigRow][bigCol] != '.') {
                    // Board already won/drawn — show big symbol in center row
                    if (smallRow == 1)
                        out << " [" << game.metaBoard[bigRow][bigCol] << " WON]  ";
                    else
                        out << "           ";
                } else {
                    for (int smallCol = 0; smallCol < 3; smallCol++)
                        out << " [" << game.boards[idx].getCell(smallRow, smallCol) << "]";
                }

                if (bigCol < 2) out << "  |  ";
            }
            out << "\n";
        }

        if (bigRow < 2)
            out << "      -----------+---------------+-----------\n";
    }

    // Print the meta board summary
    out << "\n--- Meta Board ---\n";
    out << "  1  2  3\n";
    for (int i = 0; i < 3; i++) {
        out << i + 1;
        for (int j = 0; j < 3; j++)
            out << " [" << game.metaBoard[i][j] << "]";
        out << "\n";
    }
    out << "------------------\n";

    return out;
}

// ----------------------------------------------------------
// 8. File Stream Operator (ofstream): save full ultimate game state
// ----------------------------------------------------------
ofstream& operator<<(ofstream& out, const ultimateTicTacToe& game) {
    ostream& base = out;
    base << game.currentPlayer << "\n";
    base << game.nextBoardRow << " " << game.nextBoardCol << "\n";
    base << game.winner << "\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            base << game.metaBoard[i][j];
            if (j < 2) base << " ";
        }
        base << "\n";
    }

    for (int i = 0; i < 9; i++)
        out << game.boards[i];

    return out;
}

// ----------------------------------------------------------
// 8. File Stream Operator (ifstream): load full ultimate game state
// ----------------------------------------------------------
ifstream& operator>>(ifstream& in, ultimateTicTacToe& game) {
    in >> game.currentPlayer;
    in >> game.nextBoardRow >> game.nextBoardCol;
    in >> game.winner;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            in >> game.metaBoard[i][j];

    for (int i = 0; i < 9; i++)
        in >> game.boards[i];

    return in;
}

// ----------------------------------------------------------
// Save ultimate game state to UltimateGame.txt
// ----------------------------------------------------------
void ultimateTicTacToe::saveGame() const {
    ofstream file("saves/UltimateGame.txt");
    if (!file) {
        cout << "Error: Could not open saves/UltimateGame.txt for saving.\n";
        return;
    }
    file << *this;
    file.close();
    cout << "Game saved to saves/UltimateGame.txt\n";
}

// ----------------------------------------------------------
// Load ultimate game state from UltimateGame.txt
// ----------------------------------------------------------
bool ultimateTicTacToe::loadGame() {
    ifstream file("saves/UltimateGame.txt");
    if (!file) return false;
    file >> *this;
    file.close();
    cout << "Game loaded from saves/UltimateGame.txt\n";
    return true;
}
