#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include "Camera.h"
#include <math.h>

Camera camera;

int keyStates[256] = {0};
int mouseLeftDown = 0;
int lastMouseX, lastMouseY;

GLuint cubeTexture = 0;
GLuint helpTexture = 0;
int showHelp = 0; 
float glassAlpha = 0.3f;
int buttonPressed = 0;  // 0 = nincs lenyomva, 1 = lenyomva


void handleKeys() {
    float speed = 0.1f;
    if (keyStates['w']) Camera_Move(&camera, speed, 0, 0);
    if (keyStates['s']) Camera_Move(&camera, -speed, 0, 0);
    if (keyStates['a']) Camera_Move(&camera, 0, -speed, 0);
    if (keyStates['d']) Camera_Move(&camera, 0, speed, 0);
    if (keyStates[' ']) Camera_Move(&camera, 0, 0, speed);
    if (keyStates['c']) Camera_Move(&camera, 0, 0, -speed);

    if (camera.posY < 1.0f) {
        camera.posY = 1.0f;
    }
}

void drawTexturedCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluCylinder(quad, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(quad);
}

void drawTexturedDisk(float innerRadius, float outerRadius, int slices, int loops) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluDisk(quad, innerRadius, outerRadius, slices, loops);
    gluDeleteQuadric(quad);
}

void drawGlass() {
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glEnable(GL_TEXTURE_2D);

    //glColor4f(1.0f, 1.0f, 1.0f, 0.3f); // 30%-os átlátszóság
    glColor4f(0.9f, 0.9f, 1.0f, glassAlpha); // hideg üveghatás

    glPushMatrix();
    glScalef(9.0f, 9.0f, 9.0f); // 9x nagyobb méret

    // Szélesebb test (henger), nagyobb sugár
    glRotatef(-90, 1, 0, 0);
    drawTexturedCylinder(1.6f, 1.3f, 3.0f, 32, 1);

    // Alja (korong)
    drawTexturedDisk(0.0f, 1.6f, 32, 1);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void drawButton() {
    glPushMatrix();

    // Pohár közepe, lenyomás esetén egy kicsit lejjebb mozgatjuk Y irányban
    glTranslatef(0.0f, buttonPressed ? -0.05f : 0.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f); // Piros gomb

    glRotatef(-90, 1, 0, 0); // A hengert függőleges irányba fordítjuk

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_FALSE); // Nem textúrázott

    // Henger oldalfal
    gluCylinder(quad, 2.0f, 1.3f, 1.0f, 32, 1);

    // Alja (alsó korong)
    glPushMatrix();
        glRotatef(180, 1, 0, 0); // Lefelé nézzen
        gluDisk(quad, 0.0f, 2.0f, 32, 1);
    glPopMatrix();

    // Teteje (felső korong)
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f); // Magasságba mozgatjuk
        gluDisk(quad, 0.0f, 1.3f, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();
}

void checkButtonPress() {
    float dx = camera.posX - 0.0f;
    float dy = camera.posY - 0.0f;
    float dz = camera.posZ - 0.0f;
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);

    if (!buttonPressed && distance < 1.0f) {  // pl. 1.0 egység
        buttonPressed = 1;
        printf("Gomb lenyomva!\n");
    }
}

void display() {
    glClearColor(0.0f, 0.07f, 0.27f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    Camera_ApplyView(&camera);

    drawGlass();

    drawButton();

    if (showHelp && helpTexture != 0) {
        // Átváltás 2D-re
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        gluOrtho2D(0, w, 0, h);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, helpTexture);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(1, 1, 1, 1);

        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(0, 0);
            glTexCoord2f(1, 0); glVertex2f(w, 0);
            glTexCoord2f(1, 1); glVertex2f(w, h);
            glTexCoord2f(0, 1); glVertex2f(0, h);
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = 1;
    if (key == '+') {
        glassAlpha += 0.05f;
        if (glassAlpha > 1.0f) glassAlpha = 1.0f;
        glutPostRedisplay();
    } else if (key == '-') {
        glassAlpha -= 0.05f;
        if (glassAlpha < 0.05f) glassAlpha = 0.05f;
        glutPostRedisplay();
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = 0;
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = 1;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mouseLeftDown = 0;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mouseLeftDown) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        float sensitivity = 0.2f;
        Camera_Rotate(&camera, dx * sensitivity, -dy * sensitivity);
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void timer(int val) {
    handleKeys();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
    checkButtonPress();
}

void specialKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        showHelp = !showHelp;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Kamera SOIL + FreeGLUT - Glass");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Camera_Init(&camera, 1.0f, 5.0f, 30.0f);

    cubeTexture = SOIL_load_OGL_texture(
        "glass.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (cubeTexture == 0) {
        printf("Nem sikerult betolteni a cube.png texturat: '%s'\n", SOIL_last_result());
        return 1;
    }

    helpTexture = SOIL_load_OGL_texture(
        "help.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (helpTexture == 0) {
        printf("Nem sikerult betolteni a help.png texturat: '%s'\n", SOIL_last_result());
        // Nem feltétlenül kell kilépni, csak nem lesz help kép
    }

    glutSpecialFunc(specialKeyDown);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
