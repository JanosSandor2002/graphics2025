#define SDL_MAIN_HANDLED
#include "app.h"

#include <stdio.h>

/**
 * Main function
 */
int main(void)
{
    App app;

    init_app(&app, 800, 600);
    while (app.is_running) {
        handle_app_events(&app);
        update_app(&app);
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
