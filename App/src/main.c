#define SDL_MAIN_HANDLED
#include "app.h"
#include "stb_easy_font.h"

#include <stdio.h>

int show_help = 0;  // Flag to toggle help display

/* Function to draw text using stb_easy_font */
void draw_text(float x, float y, const char* text) {
    static char buffer[99999]; // large enough buffer
    int num_quads = stb_easy_font_print(x, y, (char*)text, NULL, buffer, sizeof(buffer));

    glColor3f(1.0f, 1.0f, 1.0f); // white color
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/* New render help function */
void render_help(void) {
    draw_text(50, 50, "Hasznalat:");
    draw_text(50, 80, "W/S: Elore / Hatra");
    draw_text(50, 110, "A/D: Balra / Jobbra");
    draw_text(50, 140, "Y/X: Fel / Le");
    draw_text(50, 170, "F1: Segitseg ki/be");
}

/**
 * Main function
 */
int main(void)
{
    App app;

    init_app(&app, 800, 600);

    while (app.is_running) {
        handle_app_events(&app);

        // F1 gomb figyelése az SDL események kezelésében
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_F1]) {
            show_help = 1;  // Megjelenítjük a segédletet
        } else {
            show_help = 0;  // Elrejtjük a segédletet
        }

        update_app(&app);
        render_app(&app);

        // Ha a segédlet aktív, megjelenítjük
        if (show_help) {
            render_help();
            SDL_GL_SwapWindow(app.window);  // Biztosítjuk, hogy megjelenjen
        }
    }
    
    destroy_app(&app);

    return 0;
}
