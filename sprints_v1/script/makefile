#Current make system
CC = gcc
BIN=./bin/
HEADER=./headers/
SOURCE=./src/
OBJ= ./obj/
OUT=./out/

all: $(BIN)/main

Setup:  
		mkdir $(OBJ) 

$(BIN)/main: $(OBJ)/dnd_all.o $(OBJ)/main.o
	$(CC) -o $(BIN)/main $(OBJ)dnd_all.o $(OBJ)main.o

$(OBJ)/dnd_all.o: $(SOURCE)/dnd_all.c
					$(CC) -c $(SOURCE)/dnd_all.c -I $(HEADER)/
					mv dnd_all.o $(OBJ)/

$(OBJ)/main.o: $(SOURCE)/main.c
					$(CC) -c $(SOURCE)/main.c -I $(HEADER)/
					mv main.o $(OBJ)/

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(BIN)main

clean:
		rm -f a.out*.o
		rm -f $(OBJ)/*
		rm -f $(BIN)/*
		@echo "Cleaning is Done!"


