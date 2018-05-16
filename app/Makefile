# October 2006
# Prof.: Silvana Rossetto
#------------------------------------------------------------

VERSION = 1.0

PUB    = pub.c
SUB    = sub.c
SERVER = server.cpp

PUB_B = pub
CLI_B = sub
SRV   = serv

#-------------------------------------------------------------
# the rest of the file should not change
INC = ../include
SRC = ../src
OBJ = ../obj
BIN = ../bin

CC = gcc -Wall
CFLAGS = -I$(INC) 
LFLAGS = -lpthread 
RM = rm

# make all
all:  $(OBJ)/mysocket.o $(BIN)/$(PUB_B) $(BIN)/$(CLI_B)

$(OBJ)/mysocket.o: $(SRC)/mysocket.c $(INC)/mysocket.h
	$(CC) -c $(CFLAGS) -o $@ $(SRC)/mysocket.c

$(BIN)/$(PUB_B): $(OBJ)/mysocket.o $(PUB)
	$(CC) $(CFLAGS) -o $@ $(OBJ)/mysocket.o $(PUB) $(LFLAGS) 

$(BIN)/$(CLI_B): $(OBJ)/mysocket.o $(SUB)
	$(CC) $(CFLAGS) -o $@ $(OBJ)/mysocket.o $(SUB) $(LFLAGS)

$(BIN)/$(SRV): $(OBJ)/mysocket.o $(SERVER)
	$(CC) $(CFLAGS) -o $@ $(OBJ)/mysocket.o $(SERVER) $(LFLAGS)
# version
version: 
	@echo $(VERSION) "(Draft Version!)" 
# clean
clean:
	$(RM) -rf $(OBJ)/*.o  $(BIN)/*
