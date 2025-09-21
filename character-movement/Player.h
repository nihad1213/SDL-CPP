#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class PlayerState {
    IDLE,
    RUNNING,
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
    
    SDL_Texture* spriteTexture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    
    PlayerState currentState;
    int currentFrame;
    Uint32 lastFrameTime;
    bool facingLeft;
    bool attackComplete;
    
    float x, y;
    float velocity;
    
    static const int FRAME_WIDTH = 96;
    static const int FRAME_HEIGHT = 84;
    static const int TOTAL_FRAMES = 8;
    static const int ANIMATION_DELAY = 100;
    static const float MOVE_SPEED;
};