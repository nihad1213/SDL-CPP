#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1800;

struct Camera {
    float x, y, z;
    float pitch, yaw;
    float speed;
    float sensitivity;

    Camera() : x(0), y(0), z(5), pitch(0), yaw(-90), speed(0.1f), sensitivity(0.1f) {}

    void moveForward(float distance) {
        x += distance * cos(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
        y += distance * sin(pitch * M_PI / 180.0f);
        z += distance * sin(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
    }

    void moveRight(float distance) {
        x += distance * cos((yaw + 90) * M_PI / 180.0f);
        z += distance * sin((yaw + 90) * M_PI / 180.0f);
    }

    void moveUp(float distance) {
        y += distance;
    }

    void rotate(float xoffset, float yoffset) {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    void applyView() {
        glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(-x, -y, -z);
    }
};

void drawCube(float x, float y, float z, float size) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-size, -size,  size);
        glVertex3f( size, -size,  size);
        glVertex3f( size,  size,  size);
        glVertex3f(-size,  size,  size);
        
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-size, -size, -size);
        glVertex3f(-size,  size, -size);
        glVertex3f( size,  size, -size);
        glVertex3f( size, -size, -size);
        
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-size,  size, -size);
        glVertex3f(-size,  size,  size);
        glVertex3f( size,  size,  size);
        glVertex3f( size,  size, -size);
        
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-size, -size, -size);
        glVertex3f( size, -size, -size);
        glVertex3f( size, -size,  size);
        glVertex3f(-size, -size,  size);
        
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f( size, -size, -size);
        glVertex3f( size,  size, -size);
        glVertex3f( size,  size,  size);
        glVertex3f( size, -size,  size);
        
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, -size,  size);
        glVertex3f(-size,  size,  size);
        glVertex3f(-size,  size, -size);
    glEnd();
    
    glPopMatrix();
}

void drawGrid(float size, int divisions) {
    glBegin(GL_LINES);
    glColor3f(0.3f, 0.3f, 0.3f);
    
    for (int i = -divisions; i <= divisions; i++) {
        float pos = i * (size / divisions);
        glVertex3f(-size, 0.0f, pos);
        glVertex3f( size, 0.0f, pos);
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f,  size);
    }
    
    glEnd();
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window* window = SDL_CreateWindow("3D Camera Demo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Camera camera;
    bool running = true;
    SDL_Event event;

    std::cout << "\n=== CAMERA CONTROLS ===" << std::endl;
    std::cout << "W/S: Move forward/backward" << std::endl;
    std::cout << "A/D: Move left/right" << std::endl;
    std::cout << "SPACE/LSHIFT: Move up/down" << std::endl;
    std::cout << "Mouse: Look around" << std::endl;
    std::cout << "ESC: Release mouse / Quit" << std::endl;
    std::cout << "=====================\n" << std::endl;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION) {
                camera.rotate(event.motion.xrel, -event.motion.yrel);
            }
        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        
        if (keyState[SDL_SCANCODE_W]) {
            camera.moveForward(camera.speed);
        }
        if (keyState[SDL_SCANCODE_S]) {
            camera.moveForward(-camera.speed);
        }
        if (keyState[SDL_SCANCODE_A]) {
            camera.moveRight(-camera.speed);
        }
        if (keyState[SDL_SCANCODE_D]) {
            camera.moveRight(camera.speed);
        }
        if (keyState[SDL_SCANCODE_SPACE]) {
            camera.moveUp(camera.speed);
        }
        if (keyState[SDL_SCANCODE_LSHIFT]) {
            camera.moveUp(-camera.speed);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera.applyView();

        drawGrid(10.0f, 10);
        
        drawCube(0.0f, 0.5f, 0.0f, 0.5f); 
        drawCube(3.0f, 0.5f, 0.0f, 0.5f); 
        drawCube(-3.0f, 0.5f, 0.0f, 0.5f);
        drawCube(0.0f, 0.5f, 3.0f, 0.5f);
        drawCube(0.0f, 0.5f, -3.0f, 0.5f);
        drawCube(3.0f, 0.5f, 3.0f, 0.5f); 
        drawCube(-3.0f, 0.5f, 3.0f, 0.5f); 
        drawCube(3.0f, 0.5f, -3.0f, 0.5f); 
        drawCube(-3.0f, 0.5f, -3.0f, 0.5f); 

        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

