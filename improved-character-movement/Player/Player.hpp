#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class PlayerState {
    IDLE,
    WALKING,
    RUNNING,
    JUMPING,
    ATTACKING
};

class Player {
public:
    Player();
    ~Player();

    bool Init(SDL_Renderer* renderer);
    void HandleInput(const Uint8* keyState);
    void Update();
    void Render(SDL_Renderer* renderer);
    void Cleanup();

private:
    void UpdateAnimation();
    void UpdatePhysics();
    SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path);
    
    SDL_Texture* spriteTexture;
    SDL_Texture* idleTexture;
    SDL_Texture* walkTexture;
    SDL_Texture* runTexture;
    SDL_Texture* jumpTexture;
    SDL_Texture* attackTexture;
    
    SDL_Rect srcRect;
    SDL_Rect destRect;
    
    PlayerState currentState;
    int currentFrame;
    Uint32 lastFrameTime;
    bool facingLeft;
    bool attackComplete;
    bool isGrounded;
    
    float x, y;
    float velocityX;
    float velocityY;
    
    static const int FRAME_WIDTH = 96;
    static const int FRAME_HEIGHT = 84;
    static const int TOTAL_FRAMES = 8;
    static const int ANIMATION_DELAY = 100;
    static const float WALK_SPEED;
    static const float RUN_SPEED;
    static const float JUMP_FORCE;
    static const float GRAVITY;
    static const int GROUND_LEVEL;
};