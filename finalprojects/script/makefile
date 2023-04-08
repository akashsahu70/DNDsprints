#Current make system
CC = gcc -DLOG_USE_COLOR
BIN=./bin/
HEADER=./inc/
SOURCE=./src/
OBJ= ./obj/
OUT=./out/

all: $(BIN)/main

Setup:  
	mkdir $(OBJ) 

$(BIN)/main: $(OBJ)/dnd_all.o $(OBJ)/main.o $(OBJ)/log.o
	$(CC) -o $(BIN)/main $(OBJ)dnd_all.o $(OBJ)main.o $(OBJ)/log.o

$(OBJ)/dnd_all.o: $(SOURCE)/dnd_all.c
	$(CC) -c $(SOURCE)/dnd_all.c -I $(HEADER)/
	mv dnd_all.o $(OBJ)/

$(OBJ)/main.o: $(SOURCE)/main.c
	$(CC) -c $(SOURCE)/main.c -I $(HEADER)/
	mv main.o $(OBJ)/

$(OBJ)/log.o: $(SOURCE)/log.c
	$(CC) -c $(SOURCE)/log.c -I $(HEADER)/
	mv log.o $(OBJ)/

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(BIN)main

clean:
	rm -f a.out*.o
	rm -f $(OBJ)/*
	rm -f $(BIN)/*
	@echo "Cleaning is Done!"
