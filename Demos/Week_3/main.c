#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PALETTE_HEIGHT 50
#define MAX_LINE_COUNT 100

typedef struct {
    Uint8 r, g, b, a;
} Color;

typedef struct {
    int x1, y1, x2, y2;
    Color color;
} Line;

Line lines[MAX_LINE_COUNT];
int line_count = 0;

Color red = {255, 0, 0, 255};
Color green = {0, 255, 0, 255};
Color blue = {0, 0, 255, 255};

Color current_color = {255, 0, 0, 255};

int start_x = -1, start_y = -1;
int rectangle_mode = 0; // 0 = szakasz, 1 = téglalap

void draw_line(SDL_Renderer *renderer, Line *line) {
    SDL_SetRenderDrawColor(renderer, line->color.r, line->color.g, line->color.b, line->color.a);
    SDL_RenderDrawLine(renderer, line->x1, line->y1, line->x2, line->y2);
}

void draw_rectangle(SDL_Renderer *renderer, Line *line) {
    SDL_SetRenderDrawColor(renderer, line->color.r, line->color.g, line->color.b, line->color.a);
    SDL_Rect rect;
    rect.x = (line->x1 < line->x2) ? line->x1 : line->x2;
    rect.y = (line->y1 < line->y2) ? line->y1 : line->y2;
    rect.w = abs(line->x2 - line->x1);
    rect.h = abs(line->y2 - line->y1);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_palette(SDL_Renderer *renderer) {
    SDL_Rect r = {0, SCREEN_HEIGHT - PALETTE_HEIGHT, SCREEN_WIDTH / 3, PALETTE_HEIGHT};
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
    SDL_RenderFillRect(renderer, &r);

    r.x = SCREEN_WIDTH / 3;
    SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
    SDL_RenderFillRect(renderer, &r);

    r.x = 2 * SCREEN_WIDTH / 3;
    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);
    SDL_RenderFillRect(renderer, &r);
}

void render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fehér háttér
    SDL_RenderClear(renderer);

    for (int i = 0; i < line_count; i++) {
        if (rectangle_mode)
            draw_rectangle(renderer, &lines[i]);
        else
            draw_line(renderer, &lines[i]);
    }

    draw_palette(renderer);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL hiba: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Szakaszok/Téglalapok rajzolása", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Ablak hiba: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer hiba: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;

                if (y > SCREEN_HEIGHT - PALETTE_HEIGHT) {
                    // Paletta kattintás
                    if (x < SCREEN_WIDTH / 3)
                        current_color = red;
                    else if (x < 2 * SCREEN_WIDTH / 3)
                        current_color = green;
                    else
                        current_color = blue;
                } else {
                    // Rajzterület kattintás
                    if (start_x == -1) {
                        start_x = x;
                        start_y = y;
                    } else {
                        if (line_count < MAX_LINE_COUNT) {
                            lines[line_count].x1 = start_x;
                            lines[line_count].y1 = start_y;
                            lines[line_count].x2 = x;
                            lines[line_count].y2 = y;
                            lines[line_count].color = current_color;
                            line_count++;
                        }
                        start_x = -1;
                        start_y = -1;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    rectangle_mode = !rectangle_mode; // Space vált szakasz/téglalap között
                }
            }
        }

        render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
