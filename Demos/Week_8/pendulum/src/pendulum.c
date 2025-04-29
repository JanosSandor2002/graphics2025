#include "pendulum.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_ANGLE 40.0    // fokban
#define PERIOD 6.0        // teljes oda-vissza lengés (3+3 másodperc)

void init_pendulum(Pendulum* p) {
    p->angle = 0.0;
    p->time = 0.0;
}

void update_pendulum(Pendulum* p, double delta_time) {
    p->time += delta_time;

    // szinusz: idő alapján a szög kiszámítása
    double omega = 2.0 * M_PI / PERIOD;  // körfrekvencia
    p->angle = MAX_ANGLE * sin(omega * p->time);
}

static void draw_cube() {
    glBegin(GL_QUADS);

    // Front
    glVertex3f(-1, -1,  1); glVertex3f(1, -1,  1);
    glVertex3f(1,  1,  1); glVertex3f(-1, 1,  1);

    // Back
    glVertex3f(-1, -1, -1); glVertex3f(-1,  1, -1);
    glVertex3f(1,  1, -1); glVertex3f(1, -1, -1);

    // Left
    glVertex3f(-1, -1, -1); glVertex3f(-1, -1,  1);
    glVertex3f(-1,  1,  1); glVertex3f(-1, 1, -1);

    // Right
    glVertex3f(1, -1, -1); glVertex3f(1,  1, -1);
    glVertex3f(1,  1,  1); glVertex3f(1, -1,  1);

    // Top
    glVertex3f(-1, 1, -1); glVertex3f(-1, 1,  1);
    glVertex3f(1, 1,  1); glVertex3f(1, 1, -1);

    // Bottom
    glVertex3f(-1, -1, -1); glVertex3f(1, -1, -1);
    glVertex3f(1, -1,  1); glVertex3f(-1, -1,  1);

    glEnd();
}

void draw_pendulum(const Pendulum* p) {
    glPushMatrix();

    // forgatás a szög szerint (Z tengely körül)
    glRotatef(p->angle, 0.0f, 0.0f, 1.0f);

    // rúd
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glEnd();

    // golyó helyett kocka
    glTranslatef(0.0f, -1.0f, 0.0f);
    glScalef(0.1f, 0.1f, 0.1f);  // méret
    draw_cube();

    glPopMatrix();
}
