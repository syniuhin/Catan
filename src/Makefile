# Makefile
CXX = g++
cc = gcc
FLAGS = -g -std=c++11
SFML_FLAGS = -framework sfml-system -framework \
			 sfml-window -framework sfml-graphics

bin: main.o game.o map.o menu.o sqlite3.o userbase.o
	$(CXX) $(SFML_FLAGS) $(FLAGS) -o $@ $^
main.o: main.cpp
	$(CXX) $(FLAGS) -c $^
game.o: game.cpp
	$(CXX) $(FLAGS) -c $^
map.o: map.cpp
	$(CXX) $(FLAGS) -c $^
menu.o: menu.cpp
	$(CXX) $(FLAGS) -c $^
sqlite3.o: sqlite3.c shell.c
	$(cc) -c $^
userbase.o: userbase.cpp
	$(CXX) $(FLAGS) -c $^
clean:
	rm *.o bin
