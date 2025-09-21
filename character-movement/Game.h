#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.h"

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();
    void Cleanup();

private:
    void HandleEvents();
    void Update();
    void Render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    bool isRunning;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
};