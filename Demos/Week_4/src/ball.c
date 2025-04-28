#include "ball.h"

#include <GL/gl.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Definiáljuk a PI-t, ha nincs jelen
#endif

void init_ball(Ball* ball, float x, float y)
{
    ball->x = x;
    ball->y = y;
    ball->radius = 50;
    ball->speed_x = 100;
    ball->speed_y = 100;
    ball->spin = 0;  // Kezdő forgás
}

void update_ball(Ball* ball, double time)
{
    ball->x += ball->speed_x * time;
    ball->y += ball->speed_y * time;
    ball->spin += 0.1 * time;  // A forgás folyamatosan növekszik
}

void set_ball_speed(Ball* ball, float speed_x, float speed_y)
{
    ball->speed_x = speed_x;
    ball->speed_y = speed_y;
}

void render_ball(Ball* ball)
{
    double angle;
    double x;
    double y;

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.9, 0.8);
    glVertex2f(0, 0);
    angle = 0;
    while (angle < 2.0 * M_PI + 1) {
        x = cos(angle) * ball->radius;
        y = sin(angle) * ball->radius;
        glVertex2f(x, y);
        angle += 0.8;
    }
    glEnd();
    glPopMatrix();
}
