#include "Player.h"
#include <iostream>

const float Player::MOVE_SPEED = 200.0f;

Player::Player() : spriteTexture(nullptr), currentState(PlayerState::IDLE),
                   currentFrame(0), lastFrameTime(0), facingLeft(false),
                   attackComplete(false), x(100), y(400), velocity(0) {
    
    srcRect = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    destRect = {(int)x, (int)y, FRAME_WIDTH, FRAME_HEIGHT};
}

Player::~Player() {
    Cleanup();
}

bool Player::Init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("sprites/RUN.png");
    if (surface == nullptr) {
        std::cerr << "Unable to load image sprites/RUN.png! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    spriteTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (spriteTexture == nullptr) {
        std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void Player::HandleInput(const Uint8* keyState) {
    if (currentState == PlayerState::ATTACKING && !attackComplete) {
        return;
    }
    
    if (keyState[SDL_SCANCODE_X]) {
        currentState = PlayerState::ATTACKING;
        currentFrame = 0;
        lastFrameTime = SDL_GetTicks();
        attackComplete = false;
        velocity = 0;
        return;
    }
    
    velocity = 0;
    bool wasIdle = (currentState == PlayerState::IDLE);
    
    if (keyState[SDL_SCANCODE_RIGHT]) {
        velocity = MOVE_SPEED;
        facingLeft = false;
        currentState = PlayerState::RUNNING;
        
        if (wasIdle) {
            currentFrame = 0;
            lastFrameTime = SDL_GetTicks();
        }
    } else if (keyState[SDL_SCANCODE_LEFT]) {
        velocity = -MOVE_SPEED;
        facingLeft = true;
        currentState = PlayerState::RUNNING;
        
        if (wasIdle) {
            currentFrame = 0;
            lastFrameTime = SDL_GetTicks();
        }
    } else {
        currentState = PlayerState::IDLE;
        currentFrame = 0;
    }
}

void Player::Update() {
    x += velocity * (16.0f / 1000.0f);
    
    if (x < 0) x = 0;
    if (x > 800 - FRAME_WIDTH) x = 800 - FRAME_WIDTH;
    
    if (y < 0) y = 0;
    if (y > 600 - FRAME_HEIGHT) y = 600 - FRAME_HEIGHT;
    
    destRect.x = (int)x;
    destRect.y = (int)y;
    
    UpdateAnimation();
}

void Player::UpdateAnimation() {
    Uint32 currentTime = SDL_GetTicks();
    
    if (currentState == PlayerState::IDLE) {
        currentFrame = 0;
    } else if (currentTime - lastFrameTime >= ANIMATION_DELAY) {
        lastFrameTime = currentTime;
        
        if (currentState == PlayerState::RUNNING) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
        } else if (currentState == PlayerState::ATTACKING) {
            currentFrame++;
            if (currentFrame >= TOTAL_FRAMES) {
                attackComplete = true;
                currentState = PlayerState::IDLE;
                currentFrame = 0;
            }
        }
    }
    
    srcRect.x = currentFrame * FRAME_WIDTH;
}

void Player::Render(SDL_Renderer* renderer) {
    if (spriteTexture == nullptr) return;
    
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, spriteTexture, &srcRect, &destRect, 0.0, nullptr, flip);
}

void Player::Cleanup() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
        spriteTexture = nullptr;
    }
}