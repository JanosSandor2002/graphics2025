#include "app.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>       // Alap OpenGL függvények
#include <GL/glu.h>      // GLUT függvények (szükség esetén)

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Cube!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    // Világítás beállítása
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };  // Fény pozíció
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // Diffúz fény színe
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Speculáris fény színe

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // Fény pozíciójának beállítása
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);    // Diffúz fény színe
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  // Speculáris fény színe
    glEnable(GL_LIGHT0);  // Engedélyezés

    glEnable(GL_LIGHTING);  // Világítás engedélyezése

    // Anyagtulajdonságok beállítása
    GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // Piros anyag szín (diffúz komponens)
    GLfloat mat_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f }; // Zöld anyag szín (speculáris komponens)
    GLfloat mat_shininess[] = { 100.0f };  // Fényvisszaverő képesség (shininess)

    // Anyagtulajdonságok beállítása
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 10
    );
}

void handle_app_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene));
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

// A kocka kirajzolása
void draw_cube() {
    glBegin(GL_QUADS);
        // Elülső oldal
        glNormal3f(0.0f, 0.0f, 1.0f); // Normál vektor
        glColor3f(1.0f, 0.0f, 0.0f);   // Szín piros
        glVertex3f(-0.5f, -0.5f, 0.5f);  // bal alsó
        glVertex3f(0.5f, -0.5f, 0.5f);   // jobb alsó
        glVertex3f(0.5f, 0.5f, 0.5f);    // jobb felső
        glVertex3f(-0.5f, 0.5f, 0.5f);   // bal felső
    
        // Hátulsó oldal
        glNormal3f(0.0f, 0.0f, -1.0f); // Normál vektor
        glColor3f(0.0f, 1.0f, 0.0f);   // Szín zöld
        glVertex3f(-0.5f, -0.5f, -0.5f); // bal alsó
        glVertex3f(0.5f, -0.5f, -0.5f);  // jobb alsó
        glVertex3f(0.5f, 0.5f, -0.5f);   // jobb felső
        glVertex3f(-0.5f, 0.5f, -0.5f);  // bal felső
    
        // Bal oldal
        glNormal3f(-1.0f, 0.0f, 0.0f); // Normál vektor
        glColor3f(0.0f, 0.0f, 1.0f);   // Szín kék
        glVertex3f(-0.5f, -0.5f, 0.5f);  // elülső bal alsó
        glVertex3f(-0.5f, -0.5f, -0.5f); // hátsó bal alsó
        glVertex3f(-0.5f, 0.5f, -0.5f);  // hátsó bal felső
        glVertex3f(-0.5f, 0.5f, 0.5f);   // elülső bal felső
    
        // Jobb oldal
        glNormal3f(1.0f, 0.0f, 0.0f); // Normál vektor
        glColor3f(1.0f, 1.0f, 0.0f);   // Szín sárga
        glVertex3f(0.5f, -0.5f, 0.5f);  // elülső jobb alsó
        glVertex3f(0.5f, -0.5f, -0.5f); // hátsó jobb alsó
        glVertex3f(0.5f, 0.5f, -0.5f);  // hátsó jobb felső
        glVertex3f(0.5f, 0.5f, 0.5f);   // elülső jobb felső
    
        // Felső oldal
        glNormal3f(0.0f, 1.0f, 0.0f); // Normál vektor
        glColor3f(1.0f, 0.0f, 1.0f);   // Szín lila
        glVertex3f(-0.5f, 0.5f, 0.5f);  // elülső bal felső
        glVertex3f(0.5f, 0.5f, 0.5f);   // elülső jobb felső
        glVertex3f(0.5f, 0.5f, -0.5f);  // hátsó jobb felső
        glVertex3f(-0.5f, 0.5f, -0.5f); // hátsó bal felső
    
        // Alsó oldal
        glNormal3f(0.0f, -1.0f, 0.0f); // Normál vektor
        glColor3f(0.0f, 1.0f, 1.0f);   // Szín cián
        glVertex3f(-0.5f, -0.5f, 0.5f);  // elülső bal alsó
        glVertex3f(0.5f, -0.5f, 0.5f);   // elülső jobb alsó
        glVertex3f(0.5f, -0.5f, -0.5f);  // hátsó jobb alsó
        glVertex3f(-0.5f, -0.5f, -0.5f); // hátsó bal alsó
    
        glEnd();    
}
