#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "ball.h"

// Ablak és idő paraméterek
int window_width = 800;
int window_height = 600;
double last_time = 0.0;

int main(int argc, char* argv[]) {
    // SDL2 inicializálása
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL2 hiba: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Pattogó Labda",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          window_width, window_height, SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "SDL2 ablak hiba: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "SDL2 OpenGL kontextus hiba: %s\n", SDL_GetError());
        return -1;
    }

    // OpenGL beállítások
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, -1.0, 1.0);  // 2D vetítés beállítása
    glEnable(GL_DEPTH_TEST);

    // Pattogó labda változók
    double ball_pos_y = -1.0;  // Kezdeti pozíció (földi szint)
    double ball_speed_y = 0.0;  // Kezdeti sebesség (álló helyzet)
    
    // Fő ciklus
    int running = 1;
    while (running) {
        // Események feldolgozása
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Elapsed time számítása
        double current_time = SDL_GetTicks() / 1000.0;
        double elapsed_time = current_time - last_time;
        last_time = current_time;

        // Ball frissítése
        update_ball_position(&ball_pos_y, &ball_speed_y, elapsed_time);

        // OpenGL rajzolás
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Képernyő törlés
        draw_ball(ball_pos_y);  // Pattogó labda rajzolása

        // Kép frissítése
        SDL_GL_SwapWindow(window);

        // Időzítés
        SDL_Delay(16);  // 60 FPS (1000ms / 60)
    }

    // SDL2 takarítás
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
