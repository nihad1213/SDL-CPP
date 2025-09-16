CXX := g++

CXXFLAGS := -Wall -Wextra -Werror -std=c++23 $(shell sdl2-config --cflags)

LDFLAGS := $(shell sdl2-config --libs)

SRC = main.cpp
TARGET = sdl_app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)