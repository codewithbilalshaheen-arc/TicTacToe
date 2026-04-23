# Design Notes — CS112 Assignment 02: Tic Tac Toe

**Student:** [Your Name]
**Roll No:** [Your Roll Number]
**Section:** [Your Section]
**Date:** May 2026

---

## Overview

This document explains the Object-Oriented design decisions made while implementing the Basic Tic Tac Toe (`ticTacToe`) and Ultimate Tic Tac Toe (`ultimateTicTacToe`) programs. It covers class structure, memory management strategy, operator overloading, file I/O design, and trade-offs considered during development.

---

## Class: `ticTacToe`

### Data Members (Private)

| Member | Type | Purpose |
|---|---|---|
| `board` | `char**` | Dynamic 2D array storing cell values: `'X'`, `'O'`, or `'.'` |
| `N` | `int` | Board dimension — board is N×N, minimum 3 |
| `currentPlayer` | `char` | Tracks whose turn it is: `'X'` or `'O'` |
| `moveCount` | `int` | Total moves placed; used to detect a draw when `moveCount == N*N` |
| `winner` | `char` | `'X'` or `'O'` if won, `'D'` for draw, `'\0'` if game is ongoing |

All members are private to enforce **encapsulation** — external code can only interact with the board through the defined public interface.

### Constructor Decisions

**Default Constructor** creates a standard 3×3 board, initializing all cells to `'.'` (empty). This satisfies the assignment requirement and provides a sensible starting state when no size is specified (also used when loading from file before the real size is read).

**Parameterized Constructor** accepts any `N >= 3`. A guard `if (N < 3) N = 3;` silently enforces the minimum — rather than throwing an exception, which is not covered in CS112.

**Copy Constructor** performs a **deep copy**: it allocates a fresh `char**` array and copies each cell individually. A shallow copy would cause both objects to point to the same memory, resulting in double-free errors when both destructors run.

**Copy Assignment Operator** was added alongside the copy constructor (Rule of Three). It includes a self-assignment guard (`if (this == &other) return *this;`) and frees the old board before allocating a new one.

**Destructor** frees memory in reverse order: first each row (`delete[] board[i]`), then the pointer array (`delete[] board`), then sets `board = nullptr` to prevent dangling pointer access.

### Why `char**` Instead of a Flat Array?

A flat `char*` array of size `N*N` would also work, but a 2D pointer `char**` maps more naturally to the board's row/column structure (e.g. `board[row][col]`) and is a required part of the assignment specification ("the pointer will be used for dynamic allocation").

### Winner Detection (Generic for Any N)

The `checkWinner()` function checks:
1. All N rows — if all cells in a row match and are non-empty
2. All N columns — same check per column
3. Main diagonal (top-left → bottom-right) — `board[i][i]` for all `i`
4. Anti-diagonal (top-right → bottom-left) — `board[i][N-1-i]` for all `i`

This works for any board size N without any hard-coded `if` chains. A draw is detected separately in `makeMove()` when `moveCount == N*N` and no winner was found.

---

## Class: `ultimateTicTacToe`

### Composition Design

The assignment explicitly requires using an **array of objects** of class `ticTacToe` as a data member. This is the OOP principle of **Composition** ("has-a" relationship):

```
ultimateTicTacToe  ──has-a──►  ticTacToe boards[9]
```

The 9 small boards are stored as `ticTacToe* boards` (a dynamically allocated array of 9 `ticTacToe` objects). Each object manages its own 3×3 board independently, including its own winner detection, move validation, and memory.

A separate `char** metaBoard` (3×3) tracks which small boards have been won/drawn, and is used to determine the ultimate winner.

### Next-Board Constraint

After a player places their symbol at cell `(smallRow, smallCol)` inside a sub-board, the **opponent is sent to the sub-board at position `(smallRow, smallCol)`** in the big grid. This is the core rule of Ultimate Tic Tac Toe.

If that target sub-board is already won or drawn, the opponent gets a free choice (represented by `nextBoardRow = -1, nextBoardCol = -1`).

### Memory Management

The destructor calls `delete[] boards` — this triggers each `ticTacToe` object's own destructor automatically, which frees its internal `char**` board. The `metaBoard` rows are deleted individually before the pointer array. Both pointers are set to `nullptr` afterwards.

The copy constructor performs a **deep copy** of both the `boards` array (using `ticTacToe`'s copy assignment operator) and the `metaBoard` (cell by cell). This prevents aliasing bugs when copying game states.

---

## Operator Overloading

### `operator<<` (ostream — console output)

Used to print the board in a readable grid format with row and column labels. Declared as a `friend` function so it can access the private `board` array directly. Returning `ostream&` allows chaining (e.g. `cout << game << "\n"`).

### `operator<<` (ofstream — file save)

A separate overload for `ofstream` is required to save game state to a file. It writes metadata (N, currentPlayer, moveCount, winner) followed by the board cells, all as plain text. The `ostream& base = out;` cast is used internally to avoid ambiguity with recursive calls.

### `operator>>` (ifstream — file load)

Reads back the saved format in the exact order it was written. If the board size differs from the current object's size, memory is reallocated to fit the new size before reading cells.

---

## File I/O Design

### BasicGame.txt Format

```
3          ← N (board size)
X          ← currentPlayer
4          ← moveCount
[null]     ← winner ('\0' = ongoing)
X O .      ← board row 0
. X .      ← board row 1
O . .      ← board row 2
```

### UltimateGame.txt Format

```
O          ← currentPlayer
0 0        ← nextBoardRow nextBoardCol (-1 -1 = free)
[null]     ← winner
. . .      ← metaBoard row 0
. . .      ← metaBoard row 1
. . .      ← metaBoard row 2
[repeated 9 times: full ticTacToe save block for each sub-board]
```

The save/load is triggered by typing `0 0` (basic) or `0 0 0 0` (ultimate) during play. At program start, the menu offers to load the saved game or start fresh.

---

## Trade-offs and Limitations

**No exception handling** — invalid input is handled with `while` loops and `cin.fail()` checks rather than `try/catch`, keeping the code within CS112 scope.

**Save overwrites** — only one save slot exists per game type. A future improvement would be timestamped save files or numbered slots.

**Ultimate Tic Tac Toe draw rule** — when all 9 sub-boards are resolved but no player has 3 in a row on the meta board, the game is declared a draw. An alternative rule counts total wins per player, but the simpler approach was chosen for clarity.

**Board printing for large N** — the `operator<<` for `ticTacToe` works for any N but may wrap on terminals narrower than `3*N` characters. This is acceptable since the assignment tests N = 3 and N = 4 primarily.

---

## Summary of OOP Concepts Applied

| OOP Concept | How Applied |
|---|---|
| Encapsulation | All data members private; access via public methods only |
| Abstraction | `makeMove()` hides winner detection and player-switching logic |
| Composition | `ultimateTicTacToe` owns an array of `ticTacToe` objects |
| Constructor Overloading | Default (3×3) and Parameterized (N×N) constructors |
| Copy Constructor | Deep copy of dynamic `char**` memory |
| Copy Assignment Operator | Rule of Three — needed alongside copy constructor |
| Destructor | Proper `delete[]` deallocation; no leaks or dangling pointers |
| Operator Overloading | `<<` for console and file output; `>>` for file input |
| Friend Functions | Stream operators access private members cleanly |
| Dynamic Memory | `new`/`delete` for `char**` board and `ticTacToe*` array |
