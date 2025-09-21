#include "Game.h"
#include <iostream>

int main() {
    Game game;

    if (!game.Init()) {
        std::cerr << "Failed to initialize the game" << std::endl;
        return -1;
    }

    game.Run();
    game.Cleanup();

    return 0;
}