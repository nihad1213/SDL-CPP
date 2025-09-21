CXX = g++


CXXFLAGS = -std=c++11 -Wall -Wextra


LIBS = -lSDL2 -lSDL2_image


SOURCES = main.cpp Game.cpp Player.cpp

OBJECTS = $(SOURCES:.cpp=.o)

TARGET = knight_game

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all run clean