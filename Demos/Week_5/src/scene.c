#include "scene.h"
#include <GL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>  // GLU könyvtár
#include <math.h> 

void init_scene(Scene* scene) {
    (void)scene;
}

void update_scene(Scene* scene) {
    (void)scene;
}

void render_scene(const Scene* scene)
{
    (void)scene;
    draw_origin();
    draw_triangle();
    draw_sphere();
    draw_checkboard();
    draw_cylinder();
    draw_cone();
}

void draw_origin()
{
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);  // X-axis

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);  // Y-axis

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);  // Z-axis
    glEnd();
}

void draw_triangle()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glVertex3f(0.0f, 0.5f, 0.0f);

    glColor3f(0.0, 1.0, 1.0); // Cyan
    glVertex3f(-0.5f, -0.5f, 0.0f);

    glColor3f(1.0, 0.0, 1.0); // Magenta
    glVertex3f(0.5f, -0.5f, 0.0f);

    glEnd();
}

void draw_sphere()
{
    GLUquadric* quadric = gluNewQuadric();
    glColor3f(0.5, 0.5, 0.5);  // Grey color
    gluSphere(quadric, 0.2, 10, 10);  // Small tessellated sphere
    gluDeleteQuadric(quadric);
}

void draw_checkboard()
{
    glPushMatrix();
    glTranslatef(-1.0, -1.0, 0.0);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0) {
                glColor3f(0.8, 0.8, 0.8); // Light squares
            } else {
                glColor3f(0.2, 0.2, 0.2); // Dark squares
            }
            glBegin(GL_QUADS);
            glVertex3f(i * 0.25f, j * 0.25f, 0.0f);
            glVertex3f((i + 1) * 0.25f, j * 0.25f, 0.0f);
            glVertex3f((i + 1) * 0.25f, (j + 1) * 0.25f, 0.0f);
            glVertex3f(i * 0.25f, (j + 1) * 0.25f, 0.0f);
            glEnd();
        }
    }
    glPopMatrix();
}

void draw_cylinder()
{
    float radius = 0.5f;
    float height = 1.0f;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 360; i += 10) {
        float angle = degree_to_radian(i);
        glVertex3f(radius * cos(angle), radius * sin(angle), 0.0f);
        glVertex3f(radius * cos(angle), radius * sin(angle), height);
    }
    glEnd();
}

void draw_cone()
{
    float radius = 0.5f;
    float height = 1.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, height);  // Tip of the cone
    for (int i = 0; i <= 360; i += 10) {
        float angle = degree_to_radian(i);
        glVertex3f(radius * cos(angle), radius * sin(angle), 0.0f);
    }
    glEnd();
}
