// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// File: ticTacToe.cpp
// Description: Method definitions for the ticTacToe class
// ============================================================

#include "../include/ticTacToe.h"
#include <iostream>
#include <fstream>
using namespace std;

// ----------------------------------------------------------
// 1. Default Constructor: creates a 3x3 board
// ----------------------------------------------------------
ticTacToe::ticTacToe() : N(3), currentPlayer('X'), moveCount(0), winner('\0') {
    allocateBoard();
    initializeBoard();
}

// ----------------------------------------------------------
// 2. Parameterized Constructor: creates an NxN board
// ----------------------------------------------------------
ticTacToe::ticTacToe(int size) : N(size), currentPlayer('X'), moveCount(0), winner('\0') {
    if (N < 3) N = 3; // Enforce minimum board size
    allocateBoard();
    initializeBoard();
}

// ----------------------------------------------------------
// 3. Copy Constructor: deep copies all data members
// ----------------------------------------------------------
ticTacToe::ticTacToe(const ticTacToe& other) {
    N             = other.N;
    currentPlayer = other.currentPlayer;
    moveCount     = other.moveCount;
    winner        = other.winner;

    allocateBoard();

    // Deep copy each cell individually
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = other.board[i][j];
}

// ----------------------------------------------------------
// Copy Assignment Operator: handles self-assignment safely
// ----------------------------------------------------------
ticTacToe& ticTacToe::operator=(const ticTacToe& other) {
    if (this == &other) return *this; // Guard against self-assignment

    deallocateBoard(); // Free existing memory before overwriting

    N             = other.N;
    currentPlayer = other.currentPlayer;
    moveCount     = other.moveCount;
    winner        = other.winner;

    allocateBoard();

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = other.board[i][j];

    return *this;
}

// ----------------------------------------------------------
// 4. Destructor: properly deallocates dynamic memory
// ----------------------------------------------------------
ticTacToe::~ticTacToe() {
    deallocateBoard();
}

// ----------------------------------------------------------
// Helper: Allocate dynamic 2D array of size N x N
// ----------------------------------------------------------
void ticTacToe::allocateBoard() {
    board = new char*[N];
    for (int i = 0; i < N; i++)
        board[i] = new char[N];
}

// ----------------------------------------------------------
// Helper: Deallocate memory row by row, then the pointer array
// ----------------------------------------------------------
void ticTacToe::deallocateBoard() {
    for (int i = 0; i < N; i++)
        delete[] board[i];
    delete[] board;
    board = nullptr; // Avoid dangling pointer
}

// ----------------------------------------------------------
// Helper: Set all cells to '.' (empty)
// ----------------------------------------------------------
void ticTacToe::initializeBoard() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = '.';
}

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
int  ticTacToe::getSize()          const { return N; }
char ticTacToe::getCurrentPlayer() const { return currentPlayer; }
char ticTacToe::getWinner()        const { return winner; }
int  ticTacToe::getMoveCount()     const { return moveCount; }
char ticTacToe::getCell(int r, int c) const { return board[r][c]; }

// ----------------------------------------------------------
// 7. Check if a move is valid (in bounds and cell is empty)
// ----------------------------------------------------------
bool ticTacToe::isValidMove(int row, int col) const {
    if (row < 0 || row >= N || col < 0 || col >= N) return false;
    return board[row][col] == '.';
}

// ----------------------------------------------------------
// 6. Make a move: place symbol, update moveCount, check result
// ----------------------------------------------------------
bool ticTacToe::makeMove(int row, int col) {
    if (!isValidMove(row, col)) return false;

    board[row][col] = currentPlayer;
    moveCount++;

    if (checkWinner()) {
        winner = currentPlayer; // Current player won
    } else if (moveCount == N * N) {
        winner = 'D';           // All cells filled -> draw
    } else {
        // Switch turn to the other player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return true;
}

// ----------------------------------------------------------
// 8. Determine the winner: check rows, cols, and diagonals
//    Works generically for any N x N board
// ----------------------------------------------------------
bool ticTacToe::checkWinner() const {
    // Check each row
    for (int i = 0; i < N; i++) {
        bool rowWin = true;
        for (int j = 1; j < N; j++) {
            if (board[i][j] == '.' || board[i][j] != board[i][0]) {
                rowWin = false;
                break;
            }
        }
        if (rowWin) return true;
    }

    // Check each column
    for (int j = 0; j < N; j++) {
        bool colWin = true;
        for (int i = 1; i < N; i++) {
            if (board[i][j] == '.' || board[i][j] != board[0][j]) {
                colWin = false;
                break;
            }
        }
        if (colWin) return true;
    }

    // Check main diagonal (top-left to bottom-right)
    bool diagWin = true;
    for (int i = 1; i < N; i++) {
        if (board[i][i] == '.' || board[i][i] != board[0][0]) {
            diagWin = false;
            break;
        }
    }
    if (diagWin) return true;

    // Check anti-diagonal (top-right to bottom-left)
    bool antiWin = true;
    for (int i = 1; i < N; i++) {
        if (board[i][N-1-i] == '.' || board[i][N-1-i] != board[0][N-1]) {
            antiWin = false;
            break;
        }
    }
    if (antiWin) return true;

    return false;
}

// ----------------------------------------------------------
// Check if the game is over (winner has been decided)
// ----------------------------------------------------------
bool ticTacToe::isGameOver() const {
    return winner != '\0';
}

// ----------------------------------------------------------
// Reset board for a fresh game (same board size)
// ----------------------------------------------------------
void ticTacToe::resetGame() {
    initializeBoard();
    currentPlayer = 'X';
    moveCount     = 0;
    winner        = '\0';
}

// ----------------------------------------------------------
// 5. Stream Output Operator (cout): display board to console
// ----------------------------------------------------------
ostream& operator<<(ostream& out, const ticTacToe& game) {
    // Print column numbers header
    out << "   ";
    for (int j = 0; j < game.N; j++)
        out << " " << j + 1 << " ";
    out << "\n";

    for (int i = 0; i < game.N; i++) {
        out << " " << i + 1 << " "; // Row number
        for (int j = 0; j < game.N; j++)
            out << "[" << game.board[i][j] << "]";
        out << "\n";
    }
    return out;
}

// ----------------------------------------------------------
// 9. File Stream Operator (ofstream): save game to file
// ----------------------------------------------------------
ofstream& operator<<(ofstream& out, const ticTacToe& game) {
    // Cast to ostream to avoid ambiguity with our custom operator<<(ofstream&,ticTacToe&)
    ostream& base = out;
    base << game.N             << "\n";
    base << game.currentPlayer << "\n";
    base << game.moveCount     << "\n";
    base << game.winner        << "\n";

    for (int i = 0; i < game.N; i++) {
        for (int j = 0; j < game.N; j++) {
            base << game.board[i][j];
            if (j < game.N - 1) base << " ";
        }
        base << "\n";
    }
    return out;
}

// ----------------------------------------------------------
// 9. File Stream Operator (ifstream): load game from file
// ----------------------------------------------------------
ifstream& operator>>(ifstream& in, ticTacToe& game) {
    int newN;
    in >> newN;

    // Reallocate only if board size has changed
    if (newN != game.N) {
        game.deallocateBoard();
        game.N = newN;
        game.allocateBoard();
    }

    in >> game.currentPlayer >> game.moveCount >> game.winner;

    for (int i = 0; i < game.N; i++)
        for (int j = 0; j < game.N; j++)
            in >> game.board[i][j];

    return in;
}

// ----------------------------------------------------------
// Save current game state to BasicGame.txt
// ----------------------------------------------------------
void ticTacToe::saveGame() const {
    ofstream file("saves/BasicGame.txt");
    if (!file) {
        cout << "Error: Could not open saves/BasicGame.txt for saving.\n";
        return;
    }
    file << *this;
    file.close();
    cout << "Game saved to saves/BasicGame.txt\n";
}

// ----------------------------------------------------------
// Load game state from BasicGame.txt; returns false if not found
// ----------------------------------------------------------
bool ticTacToe::loadGame() {
    ifstream file("saves/BasicGame.txt");
    if (!file) return false;
    file >> *this;
    file.close();
    cout << "Game loaded from saves/BasicGame.txt\n";
    return true;
}
