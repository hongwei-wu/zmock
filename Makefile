# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I.

# Directories
SRC_DIR = .
EXAMPLE_DIR = example
BUILD_DIR = build
LIB_DIR = lib

# Library and executable names
LIB_NAME = zmock
LIBRARY = $(LIB_DIR)/lib$(LIB_NAME).a
EXECUTABLE = $(BUILD_DIR)/example

# Source files
LIB_SOURCES = $(SRC_DIR)/zmock.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
EXE_SOURCES = $(shell find $(EXAMPLES_DIR) -name "*.c")
EXE_OBJECTS = $(patsubst %.c,%.o,$(EXE_SOURCES))
LDFLAGS = -Wl,--wrap=func_return_void \
	  -Wl,--wrap=func_return_int \
	  -Wl,--wrap=func_return_int_ptr

# Rules
.PHONY: all clean

all: $(LIBRARY) $(EXECUTABLE)

# Build the library
$(LIBRARY): $(LIB_OBJECTS) | $(LIB_DIR)
	@ar rcs $@ $^

# Build the executable
$(EXECUTABLE): $(EXE_OBJECTS) $(LIBRARY) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -L$(LIB_DIR) -l$(LIB_NAME)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up generated files
clean:
	rm -rf $(LIB_OBJECTS) $(EXE_OBJECTS) $(LIBRARY) $(EXECUTABLE) $(LIB_DIR) $(BUILD_DIR)
