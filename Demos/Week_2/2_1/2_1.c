#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>  // SDL3 helyes #include

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_LINE_COUNT 100  // Max szakaszok száma

// Szín struktúra
typedef struct {
    Uint8 r, g, b, a;
} Color;

// Szakasz struktúra
typedef struct {
    int x1, y1, x2, y2;  // Szakasz végpontjai
    Color color;          // Szín
} Line;

Line lines[MAX_LINE_COUNT];
int line_count = 0;  // Jelenlegi szakaszok száma

// Színek definiálása
Color red = {255, 0, 0, 255};
Color green = {0, 255, 0, 255};
Color blue = {0, 0, 255, 255};

// Szekvenciális szakasz rajzolása
void draw_line(SDL_Renderer *renderer, Line *line) {
    SDL_SetRenderDrawColor(renderer, line->color.r, line->color.g, line->color.b, line->color.a);
    SDL_RenderDrawLine(renderer, line->x1, line->y1, line->x2, line->y2);
}

// Képernyő törlés és újrarajzolás
void render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Fehér háttér
    SDL_RenderClear(renderer);

    // Szakaszok kirajzolása
    for (int i = 0; i < line_count; i++) {
        draw_line(renderer, &lines[i]);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    // SDL inicializálás
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL hiba: %s\n", SDL_GetError());
        return 1;
    }

    // Ablak létrehozása
    SDL_Window *window = SDL_CreateWindow("Szakaszok rajzolása", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Ablak hiba: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer létrehozása
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer hiba: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Fő program ciklus
    SDL_Event e;
    int running = 1;
    int color_index = 0;
    Color current_color = red;  // Kezdeti szín

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Kattintás: új szakasz hozzáadása
                if (line_count < MAX_LINE_COUNT) {
                    int x = e.button.x;
                    int y = e.button.y;
                    if (line_count % 2 == 0) {
                        // Első pont
                        lines[line_count].x1 = x;
                        lines[line_count].y1 = y;
                    } else {
                        // Második pont
                        lines[line_count].x2 = x;
                        lines[line_count].y2 = y;
                        lines[line_count].color = current_color;  // Szín beállítása
                        line_count++;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                // Színválasztás billentyűleütéssel
                if (e.key.keysym.sym == SDLK_r) {
                    current_color = red;  // Piros
                } else if (e.key.keysym.sym == SDLK_g) {
                    current_color = green;  // Zöld
                } else if (e.key.keysym.sym == SDLK_b) {
                    current_color = blue;  // Kék
                }
            }
        }

        // Renderelés
        render(renderer);
    }

    // Takarítás
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
