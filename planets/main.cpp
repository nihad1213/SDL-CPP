#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Rotation angles
float sunRotation = 0.0f;
float earthOrbit = 0.0f;
float earthRotation = 0.0f;
float moonOrbit = 0.0f;
float moonRotation = 0.0f;

// Texture IDs
GLuint sunTexture = 0;
GLuint earthTexture = 0;
GLuint moonTexture = 0;

/**
 * Load Textures
 */
GLuint loadTexture(const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    
    if (!surface) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLenum format;
    if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    } else {
        format = GL_RGB;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}


/**
 * Draw sphere
 */
void drawSphere(float radius, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
}

/**
 * Initialize OpenGL
 */
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

    sunTexture = loadTexture("assets/sun.jpg");
    earthTexture = loadTexture("assets/earth.jpg");
    moonTexture = loadTexture("assets/moon.jpg");
}

/**
 * Setup Camera
 */
void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 50.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/**
 * Render
 */
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 15.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Draw Sun
    glPushMatrix();
    glRotatef(sunRotation, 0.0f, 1.0f, 0.0f);
    if (sunTexture) {
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        drawSphere(2.0f, 50, 50);
    }
    glPopMatrix();
    
    // Draw Earth
    glPushMatrix();
    glRotatef(earthOrbit, 0.0f, 1.0f, 0.0f);
    glTranslatef(8.0f, 0.0f, 0.0f);
    
    glPushMatrix();
    glRotatef(earthRotation, 0.0f, 1.0f, 0.0f);
    if (earthTexture) {
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        drawSphere(1.0f, 40, 40);
    }
    glPopMatrix();
    
    // Draw Moon
    glRotatef(moonOrbit, 0.0f, 1.0f, 0.0f);
    glTranslatef(2.5f, 0.0f, 0.0f);
    glRotatef(moonRotation, 0.0f, 1.0f, 0.0f);
    if (moonTexture) {
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        drawSphere(0.3f, 30, 30);
    }
    
    glPopMatrix();
}

/**
 * Update rotation angles
 */
void update(float deltaTime) {
    sunRotation += 10.0f * deltaTime;
    earthOrbit += 20.0f * deltaTime;
    earthRotation += 50.0f * deltaTime;
    moonOrbit += 50.0f * deltaTime;
    moonRotation += 30.0f * deltaTime;
    
    if (sunRotation >= 360.0f) sunRotation -= 360.0f;
    if (earthOrbit >= 360.0f) earthOrbit -= 360.0f;
    if (earthRotation >= 360.0f) earthRotation -= 360.0f;
    if (moonOrbit >= 360.0f) moonOrbit -= 360.0f;
    if (moonRotation >= 360.0f) moonRotation -= 360.0f;
}

/**
 * Main function
 */
int main() {
     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    SDL_Window* window = SDL_CreateWindow(
        "Solar System - Textured",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    initGL();
    setupCamera();
    
    bool running = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }
        
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        update(deltaTime);
        render();
        
        SDL_GL_SwapWindow(window);
    }
    
    if (sunTexture) glDeleteTextures(1, &sunTexture);
    if (earthTexture) glDeleteTextures(1, &earthTexture);
    if (moonTexture) glDeleteTextures(1, &moonTexture);
    
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
