#pragma once
#include <SDL2/SDL.h>

/*
* Game class manages main loop, event handling, updating and
* rendering.
*/
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
    bool isRunning;

    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
};
