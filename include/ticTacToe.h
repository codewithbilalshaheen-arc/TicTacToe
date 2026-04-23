// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// File: ticTacToe.h
// Description: Class declaration for the ticTacToe game
// ============================================================

#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <fstream>
using namespace std;

class ticTacToe {
private:
    char** board;        // Dynamic 2D array for the game board
    int N;               // Board size (N x N), minimum 3
    char currentPlayer;  // Whose turn: 'X' or 'O'
    int moveCount;       // Total moves made so far
    char winner;         // 'X', 'O', 'D' (draw), or '\0' (ongoing)

public:
    // ----------------------------------------------------------
    // Constructors & Destructor
    // ----------------------------------------------------------
    ticTacToe();                          // Default: 3x3 board
    ticTacToe(int size);                  // Parameterized: NxN board
    ticTacToe(const ticTacToe& other);    // Copy constructor
    ticTacToe& operator=(const ticTacToe& other); // Copy assignment
    ~ticTacToe();                         // Destructor

    // ----------------------------------------------------------
    // Memory Management Helpers
    // ----------------------------------------------------------
    void allocateBoard();
    void deallocateBoard();
    void initializeBoard();

    // ----------------------------------------------------------
    // Getters
    // ----------------------------------------------------------
    int  getSize()          const;
    char getCurrentPlayer() const;
    char getWinner()        const;
    int  getMoveCount()     const;
    char getCell(int r, int c) const;

    // ----------------------------------------------------------
    // Game Operations
    // ----------------------------------------------------------
    bool isValidMove(int row, int col) const;  // Check if move is valid
    bool makeMove(int row, int col);            // Place symbol & update state
    bool checkWinner()                  const;  // Check win condition (generic NxN)
    bool isGameOver()                   const;  // True if winner != '\0'
    void resetGame();                           // Reset board to empty

    // ----------------------------------------------------------
    // Stream Operators
    // ----------------------------------------------------------
    friend ostream&  operator<<(ostream& out,  const ticTacToe& game); // Print board
    friend ofstream& operator<<(ofstream& out, const ticTacToe& game); // Save to file
    friend ifstream& operator>>(ifstream& in,        ticTacToe& game); // Load from file

    // ----------------------------------------------------------
    // Save / Load
    // ----------------------------------------------------------
    void saveGame()  const; // Save state to BasicGame.txt
    bool loadGame();        // Load state from BasicGame.txt
};

#endif // TICTACTOE_H
