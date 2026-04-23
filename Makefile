# ============================================================
# CS112 – Object Oriented Programming | Assignment 02
# Air University, Islamabad
# Makefile: Build system for TicTacToe project
#
# Usage:
#   make          → compile everything → produces TicTacToe
#   make clean    → remove all compiled files
#   make run      → compile and run immediately
# ============================================================

# Compiler and flags
CXX     = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
SAV_DIR = saves

# Output executable
TARGET = TicTacToe

# Source files and object files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/ticTacToe.cpp \
       $(SRC_DIR)/ultimateTicTacToe.cpp

OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/ticTacToe.o \
       $(OBJ_DIR)/ultimateTicTacToe.o

# ============================================================
# Default target: build everything
# ============================================================
all: $(SAV_DIR) $(OBJ_DIR) $(TARGET)

# Link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile each source file into an object file
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/ticTacToe.h $(INC_DIR)/ultimateTicTacToe.h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/ticTacToe.o: $(SRC_DIR)/ticTacToe.cpp $(INC_DIR)/ticTacToe.h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/ticTacToe.cpp -o $(OBJ_DIR)/ticTacToe.o

$(OBJ_DIR)/ultimateTicTacToe.o: $(SRC_DIR)/ultimateTicTacToe.cpp $(INC_DIR)/ultimateTicTacToe.h $(INC_DIR)/ticTacToe.h
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/ultimateTicTacToe.cpp -o $(OBJ_DIR)/ultimateTicTacToe.o

# Create obj/ directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create saves/ directory if it doesn't exist
$(SAV_DIR):
	mkdir -p $(SAV_DIR)

# ============================================================
# Run the program after building
# ============================================================
run: all
	./$(TARGET)

# ============================================================
# Clean up compiled files
# ============================================================
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned build files."

.PHONY: all run clean
