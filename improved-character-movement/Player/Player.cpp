#include "Player.hpp"
#include <iostream>

const float Player::WALK_SPEED = 150.0f;
const float Player::RUN_SPEED = 300.0f;
const float Player::JUMP_FORCE = -500.0f;
const float Player::GRAVITY = 1500.0f;
const int Player::GROUND_LEVEL = 516;

/**
 * Player class implementation
*/
Player::Player() : spriteTexture(nullptr), currentState(PlayerState::IDLE),
                   currentFrame(0), lastFrameTime(0), facingLeft(false),
                   attackComplete(false), isGrounded(true), x(100), y(GROUND_LEVEL), 
                   velocityX(0), velocityY(0) {
    
    srcRect = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};
    destRect = {(int)x, (int)y, FRAME_WIDTH, FRAME_HEIGHT};
}

/**
 * Player class destructor
 */
Player::~Player() {
    Cleanup();
}

/**
 * Initialize player resources
 */
bool Player::Init(SDL_Renderer* renderer) {
    idleTexture = LoadTexture(renderer, "Assets/Character/IDLE.png");
    walkTexture = LoadTexture(renderer, "Assets/Character/WALK.png");
    runTexture = LoadTexture(renderer, "Assets/Character/RUN.png");
    jumpTexture = LoadTexture(renderer, "Assets/Character/JUMP.png");
    attackTexture = LoadTexture(renderer, "Assets/Character/ATTACK 1.png");
    
    if (!idleTexture || !walkTexture || !runTexture || !jumpTexture || !attackTexture) {
        return false;
    }
    
    spriteTexture = idleTexture;
    return true;
}

SDL_Texture* Player::LoadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (surface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    
    return texture;
}

/**
 * Handle player input
 */
void Player::HandleInput(const Uint8* keyState) {
    if (currentState == PlayerState::ATTACKING && !attackComplete) {
        return;
    }
    
    if (keyState[SDL_SCANCODE_X]) {
        currentState = PlayerState::ATTACKING;
        currentFrame = 0;
        lastFrameTime = SDL_GetTicks();
        attackComplete = false;
        velocityX = 0;
        spriteTexture = attackTexture;
        return;
    }
    
    if (keyState[SDL_SCANCODE_SPACE] && isGrounded) {
        velocityY = JUMP_FORCE;
        isGrounded = false;
        currentState = PlayerState::JUMPING;
        currentFrame = 0;
        spriteTexture = jumpTexture;
    }
    
    velocityX = 0;
    bool wasIdle = (currentState == PlayerState::IDLE);
    bool isRunning = keyState[SDL_SCANCODE_LSHIFT] || keyState[SDL_SCANCODE_RSHIFT];
    
    if (keyState[SDL_SCANCODE_RIGHT]) {
        velocityX = isRunning ? RUN_SPEED : WALK_SPEED;
        facingLeft = false;
        
        if (isGrounded) {
            currentState = isRunning ? PlayerState::RUNNING : PlayerState::WALKING;
            spriteTexture = isRunning ? runTexture : walkTexture;
            
            if (wasIdle) {
                currentFrame = 0;
                lastFrameTime = SDL_GetTicks();
            }
        }
    } else if (keyState[SDL_SCANCODE_LEFT]) {
        velocityX = isRunning ? -RUN_SPEED : -WALK_SPEED;
        facingLeft = true;
        
        if (isGrounded) {
            currentState = isRunning ? PlayerState::RUNNING : PlayerState::WALKING;
            spriteTexture = isRunning ? runTexture : walkTexture;
            
            if (wasIdle) {
                currentFrame = 0;
                lastFrameTime = SDL_GetTicks();
            }
        }
    } else {
        if (isGrounded && currentState != PlayerState::ATTACKING) {
            currentState = PlayerState::IDLE;
            currentFrame = 0;
            spriteTexture = idleTexture;
        }
    }
}

/**
 * Update player state
 */
void Player::Update() {
    UpdatePhysics();
    
    if (x < 0) x = 0;
    if (x > 800 - FRAME_WIDTH) x = 800 - FRAME_WIDTH;
    
    destRect.x = (int)x;
    destRect.y = (int)y;
    
    UpdateAnimation();
}

/**
 * Update player physics
 */
void Player::UpdatePhysics() {
    float deltaTime = 16.0f / 1000.0f;
    
    x += velocityX * deltaTime;
    
    if (!isGrounded) {
        velocityY += GRAVITY * deltaTime;
    }
    
    y += velocityY * deltaTime;
    
    if (y >= GROUND_LEVEL) {
        y = GROUND_LEVEL;
        velocityY = 0;
        isGrounded = true;
        
        if (currentState == PlayerState::JUMPING) {
            if (velocityX != 0) {
                bool isRunning = (abs(velocityX) > WALK_SPEED);
                currentState = isRunning ? PlayerState::RUNNING : PlayerState::WALKING;
                spriteTexture = isRunning ? runTexture : walkTexture;
            } else {
                currentState = PlayerState::IDLE;
                spriteTexture = idleTexture;
            }
            currentFrame = 0;
        }
    }
}

/**
 * Update player animation
 */
void Player::UpdateAnimation() {
    Uint32 currentTime = SDL_GetTicks();
    
    if (currentState == PlayerState::IDLE) {
        currentFrame = 0;
    } else if (currentTime - lastFrameTime >= ANIMATION_DELAY) {
        lastFrameTime = currentTime;
        
        if (currentState == PlayerState::WALKING || currentState == PlayerState::RUNNING) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
        } else if (currentState == PlayerState::JUMPING) {
            if (currentFrame < 4) {
                currentFrame++;
            }
        } else if (currentState == PlayerState::ATTACKING) {
            currentFrame++;
            if (currentFrame >= TOTAL_FRAMES) {
                attackComplete = true;
                currentState = PlayerState::IDLE;
                currentFrame = 0;
                spriteTexture = idleTexture;
            }
        }
    }
    
    srcRect.x = currentFrame * FRAME_WIDTH;
}

/**
 * Render the player
 */
void Player::Render(SDL_Renderer* renderer) {
    if (spriteTexture == nullptr) return;
    
    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, spriteTexture, &srcRect, &destRect, 0.0, nullptr, flip);
}

/**
 * Cleanup player resources
 */
void Player::Cleanup() {
    if (idleTexture) {
        SDL_DestroyTexture(idleTexture);
        idleTexture = nullptr;
    }
    if (walkTexture) {
        SDL_DestroyTexture(walkTexture);
        walkTexture = nullptr;
    }
    if (runTexture) {
        SDL_DestroyTexture(runTexture);
        runTexture = nullptr;
    }
    if (jumpTexture) {
        SDL_DestroyTexture(jumpTexture);
        jumpTexture = nullptr;
    }
    if (attackTexture) {
        SDL_DestroyTexture(attackTexture);
        attackTexture = nullptr;
    }
    spriteTexture = nullptr;
}