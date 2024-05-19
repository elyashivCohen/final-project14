CC = gcc
CFLAGS = -Wall -ansi -pedantic
LDFLAGS = 
OBJ_DIR = object_files
SRC_DIR = src
HEADER_DIR = headers

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))
EXEC = assembler

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | dir_creation
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

dir_creation: 
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ) $(EXEC)