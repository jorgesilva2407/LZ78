CC = g++
SRC = src
INC = include
OBJ = obj
BIN = bin
RES = result
CFLAGS = -Werror -Wall -I $(INC) -c
OBJS = $(OBJ)/trie.o $(OBJ)/main.o
EXE = main.exe

$(EXE) : $(OBJS)
	$(CC) $(OBJS) -o $(BIN)/$(EXE)

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean :
	rm $(OBJ)/*.o $(BIN)/*.exe $(RES)/*
