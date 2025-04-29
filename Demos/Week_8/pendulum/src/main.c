#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdbool.h>
#include <stdio.h>

#include "pendulum.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// globális inga objektum
Pendulum pendulum;

void init_opengl()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera pozíció
    gluLookAt(0.0, 0.0, 3.0,  // kamera
              0.0, 0.0, 0.0,  // nézeti pont
              0.0, 1.0, 0.0); // felfelé

    draw_pendulum(&pendulum);
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL hiba: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Inga animáció",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (!window) {
        fprintf(stderr, "Ablak hiba: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // VSync

    init_opengl();
    init_pendulum(&pendulum);

    bool running = true;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        update_pendulum(&pendulum, delta);
        render();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
