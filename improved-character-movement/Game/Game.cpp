#include "Game.hpp"
#include <iostream>
/**
 * Game class implementation
 */
Game::Game() : window(nullptr), renderer(nullptr), background(nullptr), isRunning(false) {}

/**
 * Game class destructor
 */
Game::~Game() {
    Cleanup();
}

/**
 * Initialize the game
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

    if (!player.Init(renderer)) {
        std::cerr << "Failed to initialize player!" << std::endl;
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
        HandleEvents();
        Update();
        Render();
        SDL_Delay(16);
    }
}

/**
 * Cleanup resources
 */
void Game::Cleanup() {
    player.Cleanup();

    if (background) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

/**
 * Handle input events
 */
void Game::HandleEvents() {
    SDL_Event e;
    
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    player.HandleInput(keyState);
}

/**
 * Update game state
 */
void Game::Update() {
    player.Update();
}

/**
 * Render the game
 */
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, background, nullptr, &dst);

    player.Render(renderer);

    SDL_RenderPresent(renderer);
}