# Directories
BUILD_DIR=build
SRC_DIR=src

# Flags for compiler and linker
CFLAGS = -ansi -Wall -Wextra -Werror
LDFLAGS = -O3 -ffast-math

# File targets
# Main test binary
$(BUILD_DIR)/main: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(SRC_DIR)/* -o $@ $(LDFLAGS)

# Debug test binary
$(BUILD_DIR)/main_debug: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(SRC_DIR)/* -o $@ $(LDFLAGS) -ggdb

# Phony rules
.PHONY: clean test debug

# Clear build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Run test binary
test: $(BUILD_DIR)/main
	$(BUILD_DIR)/main

# Build debug binary
debug: $(BUILD_DIR)/main_debug
