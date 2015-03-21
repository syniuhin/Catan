# Makefile
CXX = g++
cc = gcc

BIN = sqlite game
SRC = main.cpp userbase.cpp game.cpp menu.cpp map.cpp
EXEC = bin
SFML_FLAGS = -framework sfml-system -framework sfml-window -framework sfml-graphics

all: $(BIN)
sqlite: sqlite3.c shell.c
	$(cc) -o $@ $^
game: $(SRC) sqlite3.o
	$(CXX) $(SFML_FLAGS) -std=c++11 -o $@ $^
sqlite3.o: sqlite3.c
	$(cc) -o $@ -c $^
clean:
	rm -f $(BIN)
