# Set the C++ compiler
CXX := g++

# Compiler flags: enable all warnings, treat warnings as errors, use C++23
# standard, and include SDL2 flags
CXXFLAGS := -Wall -Wextra -Werror -std=c++23 $(shell sdl2-config --cflags)

# Linker flags: include SDL2 libraries
LDFLAGS := $(shell sdl2-config --libs)

# Source files
SRC = main.cpp

# Output executable name
TARGET = sdl_app

# Running just make will build the target
# g++ -Wall -Wextra -Werror -std=c++23 -I/usr/include/SDL2 -D_REENTRANT main.cpp -o sdl_app -L/usr/lib -lSDL2
all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# First build the target, then run it
run: $(TARGET)
	./$(TARGET)

# Remove the built executable for re-building
clean:
	rm -f $(TARGET)