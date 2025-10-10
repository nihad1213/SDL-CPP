#include "Game.hpp"
#include <iostream>

// Start with no window, no renderer, not running, no background
Game::Game() : window(nullptr), renderer(nullptr), background(nullptr), isRunning(false) {}

// Destructor for cleanup
Game::~Game() {
    Cleanup();
}

/**
 * Initialize SDL, create window and renderer, load background texture
 */
bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialized! SDL ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Improved Character Movement",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface* tempSurface = IMG_Load("Assets/Background/nature_3/orig.png");
    if (!tempSurface) {
        std::cerr << "Failed to load background image! SDL Error: " << IMG_GetError() << std::endl;
        return false;
    }

    background = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!background) {
        std::cerr << "Failed to create background texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

/**
 * Main game loop
 */
void Game::Run() {
    while (isRunning) {
        // HandleEvents();
        // Update();
        Render();
        SDL_Delay(16);
    }
}

/**
 * Clean the resources
 */
void Game::Cleanup() {

}

/**
 * Handle user input events
 */
void Game::HandleEvents() {

}

/**
 * Update game state
 */
void Game::Update() {

}

/**
 * Render the current frame
 */
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, background, nullptr, &dst);

    SDL_RenderPresent(renderer);
}

