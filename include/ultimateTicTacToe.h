// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// File: ultimateTicTacToe.h
// Description: Class declaration for Ultimate Tic Tac Toe
//              Uses an array of ticTacToe objects (Composition)
// ============================================================

#ifndef ULTIMATETICTACTOE_H
#define ULTIMATETICTACTOE_H

#include "ticTacToe.h"  // Composition: array of ticTacToe objects
#include <iostream>
#include <fstream>
using namespace std;

class ultimateTicTacToe {
private:
    ticTacToe* boards;   // Array of 9 small 3x3 ticTacToe boards
    char** metaBoard;    // 3x3 board tracking winners of small boards
    char currentPlayer;  // 'X' or 'O'
    int  nextBoardRow;   // Row of board player must play in (-1 = free choice)
    int  nextBoardCol;   // Col of board player must play in (-1 = free choice)
    char winner;         // Ultimate game winner: 'X', 'O', 'D', or '\0'

    // ----------------------------------------------------------
    // Private Helpers
    // ----------------------------------------------------------
    int  boardIndex(int br, int bc) const; // Map (row,col) -> array index 0-8
    bool isBoardPlayable(int br, int bc) const; // True if board not yet won/drawn

public:
    // ----------------------------------------------------------
    // Constructors & Destructor
    // ----------------------------------------------------------
    ultimateTicTacToe();                                    // Constructor
    ultimateTicTacToe(const ultimateTicTacToe& other);      // Copy constructor
    ~ultimateTicTacToe();                                   // Destructor

    // ----------------------------------------------------------
    // Getters
    // ----------------------------------------------------------
    char getWinner()         const;
    char getCurrentPlayer()  const;
    int  getNextBoardRow()   const;
    int  getNextBoardCol()   const;
    bool isGameOver()        const;

    // ----------------------------------------------------------
    // Game Operations
    // ----------------------------------------------------------
    bool isValidMove(int bigRow, int bigCol, int smallRow, int smallCol) const;
    bool makeMove   (int bigRow, int bigCol, int smallRow, int smallCol);
    bool checkUltimateWinner() const; // Check meta-board for winner
    bool allBoardsDone()       const; // True when all 9 boards are resolved

    // ----------------------------------------------------------
    // Stream Operators
    // ----------------------------------------------------------
    friend ostream&  operator<<(ostream& out,  const ultimateTicTacToe& game); // Print 9 boards
    friend ofstream& operator<<(ofstream& out, const ultimateTicTacToe& game); // Save to file
    friend ifstream& operator>>(ifstream& in,        ultimateTicTacToe& game); // Load from file

    // ----------------------------------------------------------
    // Save / Load
    // ----------------------------------------------------------
    void saveGame()  const; // Save state to UltimateGame.txt
    bool loadGame();        // Load state from UltimateGame.txt
};

#endif // ULTIMATETICTACTOE_H
