// ============================================================
// CS112 – Object Oriented Programming | Assignment 02
// Air University, Islamabad
// Topic: Tic Tac Toe + Ultimate Tic Tac Toe
// ============================================================

#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// ============================================================
//                   CLASS: ticTacToe
// ============================================================
class ticTacToe {
private:
    char** board;   // Dynamic 2D array for the game board
    int N;          // Board size (N x N)
    char currentPlayer; // Tracks whose turn it is ('X' or 'O')
    int moveCount;      // Total moves made so far
    char winner;        // Stores winner: 'X', 'O', 'D' (draw), or '\0' (ongoing)

public:
    // ----------------------------------------------------------
    // 1. Default Constructor: creates a 3x3 board
    // ----------------------------------------------------------
    ticTacToe() : N(3), currentPlayer('X'), moveCount(0), winner('\0') {
        allocateBoard();
        initializeBoard();
    }

    // ----------------------------------------------------------
    // 2. Parameterized Constructor: creates an NxN board
    // ----------------------------------------------------------
    ticTacToe(int size) : N(size), currentPlayer('X'), moveCount(0), winner('\0') {
        if (N < 3) N = 3; // Minimum board size is 3
        allocateBoard();
        initializeBoard();
    }

    // ----------------------------------------------------------
    // 3. Copy Constructor
    // ----------------------------------------------------------
    ticTacToe(const ticTacToe& other) {
        N = other.N;
        currentPlayer = other.currentPlayer;
        moveCount = other.moveCount;
        winner = other.winner;

        allocateBoard();
        // Deep copy the board contents
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                board[i][j] = other.board[i][j];
    }

    // ----------------------------------------------------------
    // Copy Assignment Operator
    // ----------------------------------------------------------
    ticTacToe& operator=(const ticTacToe& other) {
        if (this == &other) return *this; // Self-assignment guard

        deallocateBoard();

        N = other.N;
        currentPlayer = other.currentPlayer;
        moveCount = other.moveCount;
        winner = other.winner;

        allocateBoard();
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                board[i][j] = other.board[i][j];

        return *this;
    }

    // ----------------------------------------------------------
    // 4. Destructor: deallocates dynamic memory
    // ----------------------------------------------------------
    ~ticTacToe() {
        deallocateBoard();
    }

    // ----------------------------------------------------------
    // Helper: Allocate dynamic 2D array
    // ----------------------------------------------------------
    void allocateBoard() {
        board = new char* [N];
        for (int i = 0; i < N; i++)
            board[i] = new char[N];
    }

    // ----------------------------------------------------------
    // Helper: Deallocate dynamic 2D array
    // ----------------------------------------------------------
    void deallocateBoard() {
        for (int i = 0; i < N; i++)
            delete[] board[i];
        delete[] board;
        board = nullptr;
    }

    // ----------------------------------------------------------
    // Helper: Initialize all cells to empty ('.')
    // ----------------------------------------------------------
    void initializeBoard() {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                board[i][j] = '.';
    }

    // ----------------------------------------------------------
    // Getters
    // ----------------------------------------------------------
    int getSize() const { return N; }
    char getCurrentPlayer() const { return currentPlayer; }
    char getWinner() const { return winner; }
    int getMoveCount() const { return moveCount; }
    char getCell(int r, int c) const { return board[r][c]; }

    // ----------------------------------------------------------
    // 5. Stream Output Operator: prints current board state
    // ----------------------------------------------------------
    friend ostream& operator<<(ostream& out, const ticTacToe& game) {
        // Print column numbers
        out << "   ";
        for (int j = 0; j < game.N; j++)
            out << " " << j + 1 << " ";
        out << "\n";

        for (int i = 0; i < game.N; i++) {
            // Print row number
            out << " " << i + 1 << " ";
            for (int j = 0; j < game.N; j++) {
                out << "[" << game.board[i][j] << "]";
            }
            out << "\n";
        }
        return out;
    }

    // ----------------------------------------------------------
    // 7. Check if a move is valid
    // ----------------------------------------------------------
    bool isValidMove(int row, int col) const {
        if (row < 0 || row >= N || col < 0 || col >= N) return false;
        return board[row][col] == '.';
    }

    // ----------------------------------------------------------
    // 6. Make a move: places current player's symbol
    // ----------------------------------------------------------
    bool makeMove(int row, int col) {
        if (!isValidMove(row, col)) return false;

        board[row][col] = currentPlayer;
        moveCount++;

        // Check for winner or draw after each move
        if (checkWinner()) {
            winner = currentPlayer;
        } else if (moveCount == N * N) {
            winner = 'D'; // Draw
        } else {
            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
        return true;
    }

    // ----------------------------------------------------------
    // 8. Determine winner after each move
    // ----------------------------------------------------------
    bool checkWinner() const {
        // Check all rows
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

        // Check all columns
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
        bool antiDiagWin = true;
        for (int i = 1; i < N; i++) {
            if (board[i][N - 1 - i] == '.' || board[i][N - 1 - i] != board[0][N - 1]) {
                antiDiagWin = false;
                break;
            }
        }
        if (antiDiagWin) return true;

        return false;
    }

    // ----------------------------------------------------------
    // Check if game is over
    // ----------------------------------------------------------
    bool isGameOver() const {
        return winner != '\0';
    }

    // ----------------------------------------------------------
    // 9. File Stream Operator: save game to file
    // ----------------------------------------------------------
    friend ofstream& operator<<(ofstream& out, const ticTacToe& game) {
        out << game.N << "\n";
        out << game.currentPlayer << "\n";
        out << game.moveCount << "\n";
        out << game.winner << "\n";
        for (int i = 0; i < game.N; i++) {
            for (int j = 0; j < game.N; j++) {
                out << game.board[i][j];
                if (j < game.N - 1) out << " ";
            }
            out << "\n";
        }
        return out;
    }

    // ----------------------------------------------------------
    // 9. File Stream Operator: load game from file
    // ----------------------------------------------------------
    friend ifstream& operator>>(ifstream& in, ticTacToe& game) {
        int newN;
        in >> newN;

        // Reallocate if size changed
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
    // Save game to BasicGame.txt
    // ----------------------------------------------------------
    void saveGame() const {
        ofstream file("BasicGame.txt");
        if (!file) {
            cout << "Error: Could not open file for saving.\n";
            return;
        }
        file << *this;
        file.close();
        cout << "Game saved to BasicGame.txt\n";
    }

    // ----------------------------------------------------------
    // Load game from BasicGame.txt
    // ----------------------------------------------------------
    bool loadGame() {
        ifstream file("BasicGame.txt");
        if (!file) return false;
        file >> *this;
        file.close();
        cout << "Game loaded from BasicGame.txt\n";
        return true;
    }

    // ----------------------------------------------------------
    // Reset the board for a new game
    // ----------------------------------------------------------
    void resetGame() {
        initializeBoard();
        currentPlayer = 'X';
        moveCount = 0;
        winner = '\0';
    }
};


// ============================================================
//              CLASS: ultimateTicTacToe
// ============================================================
class ultimateTicTacToe {
private:
    ticTacToe* boards;      // Array of 9 small ticTacToe boards (3x3 each)
    char** metaBoard;       // 3x3 meta board tracking winners of small boards
    char currentPlayer;     // 'X' or 'O'
    int nextBoardRow;       // Which small board row to play in next (-1 = any)
    int nextBoardCol;       // Which small board col to play in next (-1 = any)
    char winner;            // Ultimate game winner

public:
    // ----------------------------------------------------------
    // Constructor: sets up 9 small 3x3 boards
    // ----------------------------------------------------------
    ultimateTicTacToe() : currentPlayer('X'), nextBoardRow(-1), nextBoardCol(-1), winner('\0') {
        // Create 9 small boards (each 3x3)
        boards = new ticTacToe[9];

        // Allocate meta board
        metaBoard = new char* [3];
        for (int i = 0; i < 3; i++) {
            metaBoard[i] = new char[3];
            for (int j = 0; j < 3; j++)
                metaBoard[i][j] = '.';
        }
    }

    // ----------------------------------------------------------
    // Copy Constructor
    // ----------------------------------------------------------
    ultimateTicTacToe(const ultimateTicTacToe& other) {
        currentPlayer = other.currentPlayer;
        nextBoardRow = other.nextBoardRow;
        nextBoardCol = other.nextBoardCol;
        winner = other.winner;

        // Deep copy 9 boards
        boards = new ticTacToe[9];
        for (int i = 0; i < 9; i++)
            boards[i] = other.boards[i]; // Uses ticTacToe copy constructor

        // Deep copy meta board
        metaBoard = new char* [3];
        for (int i = 0; i < 3; i++) {
            metaBoard[i] = new char[3];
            for (int j = 0; j < 3; j++)
                metaBoard[i][j] = other.metaBoard[i][j];
        }
    }

    // ----------------------------------------------------------
    // Destructor
    // ----------------------------------------------------------
    ~ultimateTicTacToe() {
        delete[] boards;
        for (int i = 0; i < 3; i++)
            delete[] metaBoard[i];
        delete[] metaBoard;
        boards = nullptr;
        metaBoard = nullptr;
    }

    // ----------------------------------------------------------
    // Helper: get board index from row/col (0-2, 0-2)
    // ----------------------------------------------------------
    int boardIndex(int br, int bc) const {
        return br * 3 + bc;
    }

    // ----------------------------------------------------------
    // Helper: check if a meta board position is already won/drawn
    // ----------------------------------------------------------
    bool isBoardPlayable(int br, int bc) const {
        return metaBoard[br][bc] == '.';
    }

    // ----------------------------------------------------------
    // 4. Stream Output Operator: print all 9 boards
    // ----------------------------------------------------------
    friend ostream& operator<<(ostream& out, const ultimateTicTacToe& game) {
        out << "\n=== ULTIMATE TIC TAC TOE ===\n";
        out << "    Board(0,0)    Board(0,1)    Board(0,2)\n";

        // Print 3 rows of small boards at a time
        for (int bigRow = 0; bigRow < 3; bigRow++) {
            // Print column headers once per big row
            out << "   ";
            for (int bigCol = 0; bigCol < 3; bigCol++) {
                out << "  1  2  3  ";
                if (bigCol < 2) out << " | ";
            }
            out << "\n";

            for (int smallRow = 0; smallRow < 3; smallRow++) {
                out << "   ";
                for (int bigCol = 0; bigCol < 3; bigCol++) {
                    int idx = game.boardIndex(bigRow, bigCol);
                    // Show won boards with big symbol
                    if (game.metaBoard[bigRow][bigCol] != '.') {
                        if (smallRow == 1)
                            out << "  [" << game.metaBoard[bigRow][bigCol] << " WON] ";
                        else
                            out << "           ";
                    } else {
                        for (int smallCol = 0; smallCol < 3; smallCol++) {
                            out << " [" << game.boards[idx].getCell(smallRow, smallCol) << "]";
                        }
                    }
                    if (bigCol < 2) out << " | ";
                }
                out << "\n";
            }
            if (bigRow < 2) {
                out << "   ----------+-----------+----------\n";
            }
        }

        // Show meta board summary
        out << "\n--- Meta Board (Big Picture) ---\n";
        out << "  1  2  3\n";
        for (int i = 0; i < 3; i++) {
            out << i + 1;
            for (int j = 0; j < 3; j++)
                out << " [" << game.metaBoard[i][j] << "]";
            out << "\n";
        }
        out << "--------------------------------\n";

        return out;
    }

    // ----------------------------------------------------------
    // 6. Check if a move is valid in ultimate game
    // ----------------------------------------------------------
    bool isValidMove(int bigRow, int bigCol, int smallRow, int smallCol) const {
        // Check bounds
        if (bigRow < 0 || bigRow >= 3 || bigCol < 0 || bigCol >= 3) return false;
        if (smallRow < 0 || smallRow >= 3 || smallCol < 0 || smallCol >= 3) return false;

        // If constrained to specific board
        if (nextBoardRow != -1 && nextBoardCol != -1) {
            if (bigRow != nextBoardRow || bigCol != nextBoardCol) return false;
        }

        // Target board must still be playable
        if (!isBoardPlayable(bigRow, bigCol)) return false;

        // Cell inside that board must be empty
        return boards[boardIndex(bigRow, bigCol)].isValidMove(smallRow, smallCol);
    }

    // ----------------------------------------------------------
    // 5. Make a move (considering last opponent move)
    // ----------------------------------------------------------
    bool makeMove(int bigRow, int bigCol, int smallRow, int smallCol) {
        if (!isValidMove(bigRow, bigCol, smallRow, smallCol)) return false;

        int idx = boardIndex(bigRow, bigCol);
        boards[idx].makeMove(smallRow, smallCol);

        // Update meta board if small board is finished
        char smallWinner = boards[idx].getWinner();
        if (smallWinner != '\0') {
            metaBoard[bigRow][bigCol] = smallWinner; // 'X', 'O', or 'D'
        }

        // 7. Check if ultimate game is won
        if (checkUltimateWinner()) {
            winner = currentPlayer;
        } else if (allBoardsDone()) {
            winner = 'D'; // Ultimate draw
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

        // Determine next board (where smallRow/smallCol points)
        if (isBoardPlayable(smallRow, smallCol)) {
            nextBoardRow = smallRow;
            nextBoardCol = smallCol;
        } else {
            // That board is done; player can go anywhere
            nextBoardRow = -1;
            nextBoardCol = -1;
        }

        return true;
    }

    // ----------------------------------------------------------
    // 7. Check if ultimate game has a winner (on meta board)
    // ----------------------------------------------------------
    bool checkUltimateWinner() const {
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
        // Check diagonals
        if (metaBoard[0][0] == p && metaBoard[1][1] == p && metaBoard[2][2] == p) return true;
        if (metaBoard[0][2] == p && metaBoard[1][1] == p && metaBoard[2][0] == p) return true;

        return false;
    }

    // ----------------------------------------------------------
    // Check if all small boards are resolved
    // ----------------------------------------------------------
    bool allBoardsDone() const {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (metaBoard[i][j] == '.') return false;
        return true;
    }

    // ----------------------------------------------------------
    // Getters
    // ----------------------------------------------------------
    char getWinner() const { return winner; }
    char getCurrentPlayer() const { return currentPlayer; }
    int getNextBoardRow() const { return nextBoardRow; }
    int getNextBoardCol() const { return nextBoardCol; }
    bool isGameOver() const { return winner != '\0'; }

    // ----------------------------------------------------------
    // 8. File stream operator: save ultimate game
    // ----------------------------------------------------------
    friend ofstream& operator<<(ofstream& out, const ultimateTicTacToe& game) {
        out << game.currentPlayer << "\n";
        out << game.nextBoardRow << " " << game.nextBoardCol << "\n";
        out << game.winner << "\n";

        // Save meta board
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                out << game.metaBoard[i][j];
                if (j < 2) out << " ";
            }
            out << "\n";
        }

        // Save each of the 9 small boards
        for (int i = 0; i < 9; i++) {
            out << game.boards[i]; // Uses ticTacToe ofstream operator
        }
        return out;
    }

    // ----------------------------------------------------------
    // 8. File stream operator: load ultimate game
    // ----------------------------------------------------------
    friend ifstream& operator>>(ifstream& in, ultimateTicTacToe& game) {
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
    // Save to UltimateGame.txt
    // ----------------------------------------------------------
    void saveGame() const {
        ofstream file("UltimateGame.txt");
        if (!file) {
            cout << "Error: Could not open file for saving.\n";
            return;
        }
        file << *this;
        file.close();
        cout << "Game saved to UltimateGame.txt\n";
    }

    // ----------------------------------------------------------
    // Load from UltimateGame.txt
    // ----------------------------------------------------------
    bool loadGame() {
        ifstream file("UltimateGame.txt");
        if (!file) return false;
        file >> *this;
        file.close();
        cout << "Game loaded from UltimateGame.txt\n";
        return true;
    }
};


// ============================================================
//         PROBLEM 1: Basic Tic Tac Toe Game Loop
// ============================================================
void playBasicTicTacToe() {
    cout << "\n========================================\n";
    cout << "       BASIC TIC TAC TOE\n";
    cout << "========================================\n";
    cout << "1. Start New Game\n";
    cout << "2. Load Saved Game\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    int boardSize = 3;
    ticTacToe* game = nullptr;

    if (choice == 2) {
        game = new ticTacToe(); // Default 3x3
        if (!game->loadGame()) {
            cout << "No saved game found. Starting new game.\n";
            delete game;
            game = nullptr;
            choice = 1;
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

    cout << "\nGame started! Enter row and column (1-indexed).\n";
    cout << "Type 0 0 to save and exit.\n\n";

    while (!game->isGameOver()) {
        cout << *game;
        cout << "Player " << game->getCurrentPlayer() << "'s turn.\n";
        cout << "Enter row and column (or 0 0 to save & exit): ";

        int row, col;
        cin >> row >> col;

        // Save and exit option
        if (row == 0 && col == 0) {
            game->saveGame();
            delete game;
            return;
        }

        // Convert to 0-indexed
        row--; col--;

        if (!game->makeMove(row, col)) {
            cout << "Invalid move! Cell is occupied or out of bounds. Try again.\n";
        }
    }

    // Game over
    cout << *game;
    if (game->getWinner() == 'D')
        cout << "\n*** It's a DRAW! Well played both! ***\n";
    else
        cout << "\n*** Player " << game->getWinner() << " WINS! Congratulations! ***\n";

    delete game;
}


// ============================================================
//        PROBLEM 2: Ultimate Tic Tac Toe Game Loop
// ============================================================
void playUltimateTicTacToe() {
    cout << "\n========================================\n";
    cout << "       ULTIMATE TIC TAC TOE\n";
    cout << "========================================\n";
    cout << "1. Start New Game\n";
    cout << "2. Load Saved Game\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    ultimateTicTacToe* game = new ultimateTicTacToe();

    if (choice == 2) {
        if (!game->loadGame()) {
            cout << "No saved game found. Starting new game.\n";
        }
    }

    cout << "\nUltimate Tic Tac Toe started!\n";
    cout << "Enter: BigRow BigCol SmallRow SmallCol (all 1-indexed)\n";
    cout << "Type 0 0 0 0 to save and exit.\n\n";

    while (!game->isGameOver()) {
        cout << *game;

        // Show constraint
        int nbr = game->getNextBoardRow();
        int nbc = game->getNextBoardCol();
        if (nbr != -1 && nbc != -1)
            cout << "Player " << game->getCurrentPlayer()
                 << " MUST play in Board (" << nbr + 1 << "," << nbc + 1 << ")\n";
        else
            cout << "Player " << game->getCurrentPlayer()
                 << " can play in ANY available board.\n";

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
        br--; bc--; sr--; sc--;

        if (!game->makeMove(br, bc, sr, sc)) {
            cout << "Invalid move! Check board constraints and try again.\n";
        }
    }

    // Game over
    cout << *game;
    if (game->getWinner() == 'D')
        cout << "\n*** It's a DRAW! What a game! ***\n";
    else
        cout << "\n*** Player " << game->getWinner() << " WINS the ULTIMATE game! Congratulations! ***\n";

    delete game;
}


// ============================================================
//                       MAIN MENU
// ============================================================
int main() {
    cout << "========================================\n";
    cout << "   CS112 OOP Assignment 02\n";
    cout << "   Tic Tac Toe Games\n";
    cout << "   Air University, Islamabad\n";
    cout << "========================================\n";

    int choice;
    do {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Play Basic Tic Tac Toe (Problem 01)\n";
        cout << "2. Play Ultimate Tic Tac Toe (Problem 02)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
        case 1:
            playBasicTicTacToe();
            break;
        case 2:
            playUltimateTicTacToe();
            break;
        case 3:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    } while (choice != 3);

    return 0;
}
