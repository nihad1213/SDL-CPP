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

// Load Textures

/**
 * Draw sphere
 */
void drawSphere(float radius, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);
}



