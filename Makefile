CC = gcc
CFLAGS = -Wall -g
LDFLAGS =
LIBS = -lcheck -lm -lpthread -lrt -lsubunit  # Adicionando libsubunit

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
EXAMPLE_DIR = examples

# Arquivos
EXAMPLE_TARGET = $(BIN_DIR)/example
TEST_TARGET = $(BIN_DIR)/test_lz77
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
EXAMPLE_SOURCES = $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:$(EXAMPLE_DIR)/%.c=$(OBJ_DIR)/%.o)

all: build $(EXAMPLE_TARGET) $(TEST_TARGET)

build:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(EXAMPLE_TARGET): $(EXAMPLE_OBJECTS) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
	$(CC) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXAMPLE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all build clean test
