# CS112 – OOP Assignment 02: Tic Tac Toe

**Air University, Islamabad — Department of Creative Technologies**
**Course:** CS112 Object Oriented Programming | Spring 2026

---

## Overview

This project implements two C++ games using Object-Oriented Programming:

- **Problem 01** — Generic N×N Tic Tac Toe (class `ticTacToe`)
- **Problem 02** — Ultimate Tic Tac Toe using composition of `ticTacToe` objects (class `ultimateTicTacToe`)

---

## Project Structure

```
CS112_Assignment02/
├── include/
│   ├── ticTacToe.h              # Class declaration for basic game
│   └── ultimateTicTacToe.h      # Class declaration for ultimate game
├── src/
│   ├── ticTacToe.cpp            # Method definitions for basic game
│   ├── ultimateTicTacToe.cpp    # Method definitions for ultimate game
│   └── main.cpp                 # Entry point and game loops
├── saves/                       # Auto-created; stores save files
│   ├── BasicGame.txt            # Save state for basic game
│   └── UltimateGame.txt         # Save state for ultimate game
├── obj/                         # Auto-created; stores compiled .o files
├── Makefile                     # Build system
├── TicTacToe.cpp                # Single-file alternative version
└── README.md                    # This file
```

---

## How to Compile

### Using Makefile (recommended)

```bash
make          # Compiles everything, produces ./TicTacToe
make run      # Compiles and runs immediately
make clean    # Removes compiled files
```

### Manual Compilation

```bash
g++ -Wall -Wextra -std=c++17 -Iinclude \
    src/ticTacToe.cpp \
    src/ultimateTicTacToe.cpp \
    src/main.cpp \
    -o TicTacToe
```

### Single-file version

```bash
g++ -Wall -Wextra TicTacToe.cpp -o TicTacToe
```

---

## How to Play

### Problem 01 — Basic Tic Tac Toe

- Choose board size N (minimum 3)
- Players take turns entering **row** and **column** (1-indexed)
- Win by filling N in a row — horizontally, vertically, or diagonally
- Type `0 0` to save the game and exit at any time

### Problem 02 — Ultimate Tic Tac Toe

- There are 9 small 3×3 boards arranged in a 3×3 grid
- Enter **BigRow BigCol SmallRow SmallCol** (all 1-indexed)
- Your move's position in the small board determines which big board your opponent must play in next
- Win three small boards in a row (on the meta board) to win the ultimate game
- Type `0 0 0 0` to save and exit at any time

---

## OOP Concepts Used

| Concept | Where Used |
|---|---|
| Classes & Objects | `ticTacToe`, `ultimateTicTacToe` |
| Encapsulation | All data members are `private` with public methods |
| Default Constructor | `ticTacToe()` → 3×3 board |
| Parameterized Constructor | `ticTacToe(int N)` → N×N board |
| Copy Constructor | Deep copies dynamic `char**` array |
| Copy Assignment Operator | Handles self-assignment, avoids leaks |
| Destructor | Frees `char**` arrays with `delete[]` |
| Composition | `ultimateTicTacToe` contains array of `ticTacToe` objects |
| Operator Overloading | `<<` for `ostream`, `ofstream`; `>>` for `ifstream` |
| File I/O | Save/load via `BasicGame.txt` and `UltimateGame.txt` |
| Dynamic Memory | `new`/`delete` with no leaks or dangling pointers |

---

## Save File Format

### BasicGame.txt

```
3           ← Board size N
X           ← Current player
4           ← Move count
\0          ← Winner ('\0' = ongoing, 'X'/'O'/'D')
X . O       ← Board rows (space-separated cells)
. X .
O . .
```

### UltimateGame.txt

```
X           ← Current player
1 2         ← Next forced board (row col), -1 -1 = free
\0          ← Ultimate winner
. X .       ← Meta board rows
. . .
O . .
3           ← Sub-board 0: size
...         ← Sub-board 0: full state (same as BasicGame format)
(× 9 boards total)
```
