#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Camera.h"
#include "Model.h"
#include "Fog.h"

int windowWidth = 800;
int windowHeight = 600;
float buttonPressAnim = 0.0f;
bool buttonRemove = false;
float glassRotation = 0.0f;

SDL_Window* window = NULL;
SDL_GLContext glContext;

Camera camera;
Model* glass = NULL;
Model* button = NULL;

GLuint glassTexture = 0;
GLuint helpTexture = 0;

int keyStates[SDL_NUM_SCANCODES] = {0};
int mouseLeftDown = 0;
int lastMouseX, lastMouseY;
int showHelp = 0;
float glassAlpha = 0.3f;

void handleKeys() {
    float speed = 0.1f;
    if (keyStates[SDL_SCANCODE_W]) Camera_Move(&camera, speed, 0, 0);
    if (keyStates[SDL_SCANCODE_S]) Camera_Move(&camera, -speed, 0, 0);
    if (keyStates[SDL_SCANCODE_A]) Camera_Move(&camera, 0, -speed, 0);
    if (keyStates[SDL_SCANCODE_D]) Camera_Move(&camera, 0, speed, 0);
    if (keyStates[SDL_SCANCODE_SPACE]) Camera_Move(&camera, 0, 0, speed);
    if (keyStates[SDL_SCANCODE_C]) Camera_Move(&camera, 0, 0, -speed);

    if (camera.posY < 1.0f) camera.posY = 1.0f;
}

void updateButtonPressAnim() {
        float dx = camera.posX - 0.0f;
        float dy = camera.posY - 0.0f;
        float dz = camera.posZ - 0.0f;
        float distance = sqrtf(dx*dx + dy*dy + dz*dz);

        float target = (distance < 1.5f) ? 1.0f : 0.0f;
        float speed = 0.02f; // animáció sebessége (nagyobb gyorsabb)

        if (buttonPressAnim < target) {
            buttonPressAnim += speed;
            if (buttonPressAnim > target) buttonPressAnim = target;
        } else if (buttonPressAnim > target) {
            buttonPressAnim -= speed;
            if (buttonPressAnim < target) buttonPressAnim = target;
        }
        printf("Distance to button: %f\n", distance);

        float targetRotation = (buttonPressAnim > 0.9f) ? 90.0f : 0.0f;
        float rotSpeed = 2.0f; // fok / képkocka (állítsd lassabbra/gyorsabbra)

        if (buttonPressAnim > 0.9f && glassRotation < 90.0f) {
        float rotSpeed = 2.0f;
        glassRotation += rotSpeed;
        if (glassRotation > 90.0f) glassRotation = 90.0f;
    }
}

void display() {
    glClearColor(0.0f, 0.07f, 0.27f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Camera_ApplyView(&camera);

    // Átlátszó objektum
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    float minY = get_model_min_y(glass);
    glPushMatrix();
    glTranslatef(0.0f, -minY * 11.0f, 0.0f);
    glScalef(11.0f, 11.0f, 11.0f);

    if (glassRotation > 0.0f) {
    glRotatef(glassRotation, 1.0f, 0.0f, 0.0f);
}

    glColor4f(1.0f, 1.0f, 1.0f, glassAlpha);
    draw_model(glass);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    // Gomb kirajzolása
    if (!buttonRemove) {
    float yScale = 0.6667f * (1.0f - buttonPressAnim);

    if (yScale > 0.001f) { //még nem teljesen lenyomott
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(2.0f, yScale, 2.0f);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        draw_model(button);
        glPopMatrix();
    } else {
        buttonRemove = true; // csak akkor állítsuk be, ha valóban lenyomódott
    }
    }

    // Help overlay
    if (showHelp && helpTexture != 0) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, helpTexture);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1, 1, 1, glassAlpha);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(windowWidth, 0);
        glTexCoord2f(1, 1); glVertex2f(windowWidth, windowHeight);
        glTexCoord2f(0, 1); glVertex2f(0, windowHeight);
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    SDL_GL_SwapWindow(window);
}

void processEvents(SDL_Event* e) {
    while (SDL_PollEvent(e)) {
        switch (e->type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                keyStates[e->key.keysym.scancode] = 1;
                if (e->key.keysym.sym == SDLK_F1) showHelp = !showHelp;
                if (e->key.keysym.sym == SDLK_PLUS || e->key.keysym.sym == SDLK_KP_PLUS) {
                    glassAlpha += 0.05f;
                    if (glassAlpha > 1.0f) glassAlpha = 1.0f;
                } else if (e->key.keysym.sym == SDLK_MINUS || e->key.keysym.sym == SDLK_KP_MINUS) {
                    glassAlpha -= 0.05f;
                    if (glassAlpha < 0.05f) glassAlpha = 0.05f;
                }
                break;
            case SDL_KEYUP:
                keyStates[e->key.keysym.scancode] = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e->button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = 1;
                    lastMouseX = e->button.x;
                    lastMouseY = e->button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (e->button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                if (mouseLeftDown) {
                    int dx = e->motion.x - lastMouseX;
                    int dy = e->motion.y - lastMouseY;
                    Camera_Rotate(&camera, dx * 0.2f, -dy * 0.2f);
                    lastMouseX = e->motion.x;
                    lastMouseY = e->motion.y;
                }
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init hiba: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("SDL + OpenGL Kamera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Ablak letrehozasi hiba: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("GL context hiba: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_fog();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    Camera_Init(&camera, 1.0f, 5.0f, 30.0f);

    glass = load_model("assets/models/glass.obj");
    button = load_model("assets/models/button.obj");
    if (!glass || !button) {
        printf("Modell betoltesi hiba\n");
        return 1;
    }

    glassTexture = SOIL_load_OGL_texture("assets/textures/glass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    helpTexture = SOIL_load_OGL_texture("assets/textures/help.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

    SDL_Event e;
    while (1) {
        processEvents(&e);
        handleKeys();
        updateButtonPressAnim();
        display();
        SDL_Delay(16);  // ~60 FPS
    }

    free_model(glass);
    free_model(button);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
