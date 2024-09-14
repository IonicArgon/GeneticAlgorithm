CC = gcc
CLFAGS = -lm -Ofast
DLFAGS = -Wall -Wextra -W -pedantic -O0 -lm -I$(DIR_INC)

DIR_SRC = src
DIR_INC = include
DIR_BIN = bin

DEPS = $(DIR_INC)/functions.h
OBJS = $(DIR_BIN)/main.o $(DIR_BIN)/of.o $(DIR_BIN)/functions.o

$(DIR_BIN)/%.o: $(DIR_SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(DLFAGS)

main: $(OBJS)
	$(CC) -o $@ $^ $(DLFAGS)

.PHONY: clean

clean:
	rm -f $(DIR_BIN)/*.o main