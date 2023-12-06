# Directories
LIB_DIR := Common-Lib
LIB_BUILD_DIR := $(LIB_DIR)/lib  # Assuming libraries are built in a 'build' subdirectory of LIB_DIR
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include

# Path to the library's header files and the library file itself
LIB_INC_DIR := $(LIB_DIR)/Com
LIBS := -L$(LIB_BUILD_DIR) -lcomms  -lcore -lusb

# Compiler and linker
CC := g++
LD := g++

# Compiler and linker flags
CFLAGS := -I$(INC_DIR) -I$(LIB_DIR)/Core/Inc -I$(LIB_DIR)/Comms/Inc -I$(LIB_DIR)/Usb/Inc -Wall -g -Og
LDFLAGS := $(LIBS)

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Final executable name
EXEC := main

# Default target
all: libraries local_build

# Build libraries
libraries:
	$(MAKE) -C $(LIB_DIR)

# Build local project
local_build: $(BUILD_DIR) $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXEC)

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean target
clean:
	rm -rf $(BUILD_DIR) $(EXEC)
	$(MAKE) -C $(LIB_DIR) clean

.PHONY: all libraries local_build clean
