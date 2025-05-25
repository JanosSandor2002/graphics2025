#include "App.h"
#include <SDL2/SDL_opengl.h>
#include <SOIL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Camera.h"
#include "Model.h"
#include "Fog.h"

// Állapotváltozók
static int windowWidth = 800;
static int windowHeight = 600;
static float buttonPressAnim = 0.0f;
static bool buttonRemove = false;
static float glassRotation = 0.0f;
static float waterHeight = 1.0f;
static float waterWidth = 1.0f;

static SDL_Window* window = NULL;
static SDL_GLContext glContext;

static Camera camera;
static Model* glass = NULL;
static Model* button = NULL;
static Model* water = NULL;

static GLuint waterTexture = 0;
static GLuint glassTexture = 0;
static GLuint helpTexture = 0;

static int keyStates[SDL_NUM_SCANCODES] = {0};
static int mouseLeftDown = 0;
static int lastMouseX, lastMouseY;
static int showHelp = 0;
static float glassAlpha = 0.3f;

void App_Init() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("SDL + OpenGL Kamera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    glContext = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_fog();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    Camera_Init(&camera, 1.0f, 5.0f, 30.0f);
}

void App_LoadContent() {
    glass = load_model("assets/models/glass.obj");
    button = load_model("assets/models/button.obj");
    water = load_model("assets/models/water.obj");

    glassTexture = SOIL_load_OGL_texture("assets/textures/glass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    waterTexture = SOIL_load_OGL_texture("assets/textures/water.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0); // ha van
    helpTexture = SOIL_load_OGL_texture("assets/textures/help.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
}

void App_ProcessEvents(SDL_Event* e) {
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

void App_HandleKeys() {
    float speed = 0.1f;
    if (keyStates[SDL_SCANCODE_W]) Camera_Move(&camera, speed, 0, 0);
    if (keyStates[SDL_SCANCODE_S]) Camera_Move(&camera, -speed, 0, 0);
    if (keyStates[SDL_SCANCODE_A]) Camera_Move(&camera, 0, -speed, 0);
    if (keyStates[SDL_SCANCODE_D]) Camera_Move(&camera, 0, speed, 0);
    if (keyStates[SDL_SCANCODE_SPACE]) Camera_Move(&camera, 0, 0, speed);
    if (keyStates[SDL_SCANCODE_C]) Camera_Move(&camera, 0, 0, -speed);
    if (camera.posY < 1.0f) camera.posY = 1.0f;
}

void App_Update() {
    float dx = camera.posX;
    float dy = camera.posY;
    float dz = camera.posZ;
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);
    float target = (distance < 1.5f) ? 1.0f : 0.0f;
    float speed = 0.02f;

    if (distance > 32.0f) {
        // Reset all relevant state
        buttonPressAnim = 0.0f;
        buttonRemove = false;
        glassRotation = 0.0f;
        waterHeight = 1.0f;
        waterWidth = 1.0f;

        // Reset camera pozíció
        Camera_Init(&camera, 1.0f, 5.0f, 30.0f);
    }

    printf("Distance: %6f", distance);
    if (buttonPressAnim < target) {
        buttonPressAnim += speed;
        if (buttonPressAnim > target) buttonPressAnim = target;
    } else if (buttonPressAnim > target) {
        buttonPressAnim -= speed;
        if (buttonPressAnim < target) buttonPressAnim = target;
    }

    if (buttonPressAnim > 0.9f && glassRotation < 90.0f) {
        glassRotation += 2.0f;
        if (glassRotation > 90.0f) glassRotation = 90.0f;
    }
}

void App_Render() {
    glClearColor(0.0f, 0.07f, 0.27f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    Camera_ApplyView(&camera);

    // Glass
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
    if (glassRotation > 0.0f) glRotatef(glassRotation, 1.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, glassAlpha);
    draw_model(glass);
    glPopMatrix();

    if (glassRotation >= 90.0f && waterHeight > 0.0f) {
        float waterDecreaseSpeed = 0.002f;

        waterHeight -= waterDecreaseSpeed;
        waterWidth -= waterDecreaseSpeed / 2;
        if (waterHeight < 0.0f) waterHeight = 0.0f;
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

     // Water
    if (water && waterHeight > 0.0f) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, waterTexture);  // ha van
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glPushMatrix();
        glTranslatef(0.0f, 11.0f * waterHeight, 0.0f);
        if (glassRotation > 0.0f) glRotatef(glassRotation, 1.0f, 0.0f, 0.0f);
        glScalef(10.0f * waterWidth, 10.0f, 10.0f * waterHeight);
        glColor4f(0.2f, 0.4f, 1.0f, 0.5f);
        draw_model(water);
        glPopMatrix();

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }

    // Button
    if (!buttonRemove) {
        float yScale = 0.6667f * (1.0f - buttonPressAnim);
        if (yScale > 0.001f) {
            glPushMatrix();
            glScalef(2.0f, yScale, 2.0f);
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            draw_model(button);
            glPopMatrix();
        } else {
            buttonRemove = true;
        }
    }

    // Help
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

void App_Cleanup() {
    free_model(glass);
    free_model(water);
    free_model(button);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
