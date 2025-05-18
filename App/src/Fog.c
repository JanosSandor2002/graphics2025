#include "Fog.h"
#include <GL/gl.h>

void init_fog() {
    GLfloat fogColor[] = {0.7f, 0.7f, 0.7f, 1.0f};  // szürkés köd

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);                 // erősebb köd típus
    glFogfv(GL_FOG_COLOR, fogColor);              // köd színe
    glFogf(GL_FOG_DENSITY, 0.08f);                // ködsűrűség – nagyobb = sűrűbb
    glHint(GL_FOG_HINT, GL_NICEST);               // jobb minőség
}
