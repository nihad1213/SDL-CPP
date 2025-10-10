#include "Game.hpp"
#include <iostream>

// Start with no window, no renderer, not running, no background
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), background(nullptr) {}

// Destructor for cleanup
Game::~Game() {
    Cleanup();
}

/**
 * Initialize SDL, create window and renderer, load background texture
 */
bool Game::Init() {

}

/**
 * Main game loop
 */
void Game::Run() {

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

}

