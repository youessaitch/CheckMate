CC := g++
FLAGS := -g -Wall -Wextra
SRC := Code
OBJ := obj
BINDIR := bin
BINNAME := main
INCDIR := $(SRC)
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
BIN := $(BINDIR)/$(BINNAME).exe
RMV := rm -f -r

all: clean $(BIN)

build:$(BIN)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(FLAGS) -I$(INCDIR) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

clean:
	$(RMV) $(OBJ)/* $(BIN)