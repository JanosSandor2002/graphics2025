#include "Fog.h"
#include <GL/gl.h>

void init_fog() {
    GLfloat fogColor[] = {0.7f, 0.7f, 0.7f, 1.0f};

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.08f);
    glHint(GL_FOG_HINT, GL_NICEST);
}
