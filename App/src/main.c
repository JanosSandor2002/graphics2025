#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include "App.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init hiba: %s\n", SDL_GetError());
        return 1;
    }

    App_Init();
    App_LoadContent();

    SDL_Event e;
    while (1) {
        App_ProcessEvents(&e);
        App_HandleKeys();
        App_Update();
        App_Render();
        SDL_Delay(16);
    }

    App_Cleanup();
    return 0;
}
