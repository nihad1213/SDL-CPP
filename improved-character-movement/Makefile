# Define the compiler
CXX := g++

# Compiler Flags
CXXFLAGS := -Wall -Wextra -Werror -std=c++23 $(shell sdl2-config --cflags)

# Define source files
SRC = main.cpp Game/Game.cpp

# Output executable name
TARGET = i_character_movement

# Build the TARGET (if we just type make in terminal)
all: $(TARGET)

# Rule to build the executable
$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Run the program (if we type make run in terminal)
run: $(TARGET)
	./$(TARGET)

# Clean up build files (if we type make clean in terminal)
clean:
	rm -f $(TARGET)
