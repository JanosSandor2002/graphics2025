#include "ball.h"
#include <GL/gl.h>
#include <GL/glu.h>

double gravity = -9.8;  // Gravitációs gyorsulás

// Rajzolja a labdát a megfelelő helyen
void draw_ball(double ball_pos_y) {
    glPushMatrix();  // Az aktuális mátrix mentése

    // Áthelyezzük a labdát, hogy a megfelelő pozícióban jelenjen meg
    glTranslatef(0.0f, ball_pos_y, -5.0f);  // A Z tengelyen -5.0-ra mozgatjuk a kamerát

    // Gömb rajzolása OpenGL használatával
    GLUquadricObj* quadric = gluNewQuadric();
    gluSphere(quadric, 0.1, 20, 20);  // Gömb átmérője: 0.1
    gluDeleteQuadric(quadric);

    glPopMatrix();  // Az aktuális mátrix visszaállítása
}

// Frissíti a labda helyzetét
void update_ball_position(double* ball_pos_y, double* ball_speed_y, double elapsed_time) {
    *ball_speed_y += gravity * elapsed_time;  // Gravitáció hatása
    *ball_pos_y += *ball_speed_y * elapsed_time;  // Sebesség hatása a pozícióra

    // Ütközés a talajjal (pattogás)
    if (*ball_pos_y <= -1.0) {
        *ball_pos_y = -1.0;  // A labda visszapattan
        *ball_speed_y = -*ball_speed_y * 0.9;  // Ütközés után a sebesség csökkentése (pattogás)
    }
}
