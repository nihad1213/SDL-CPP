CXX := g++

CXXFLAGS := -Wall -Wextra -Werror -std=c++23 $(shell sdl2-config --cflags) 

LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer

SRC = main.cpp

TARGET = sdl_app

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

