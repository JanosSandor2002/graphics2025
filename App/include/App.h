#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>

void App_Init();
void App_LoadContent();
void App_ProcessEvents(SDL_Event* e);
void App_HandleKeys();
void App_Update();
void App_Render();
void App_Cleanup();

#endif
