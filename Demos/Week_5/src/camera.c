#include "camera.h"
#include "utils.h"
#include <GL/gl.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.z = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}

void update_camera(Camera* camera, double time)
{
    camera->position.x += camera->speed.x * time;
    camera->position.z += camera->speed.z * time;
}

void set_view(const Camera* camera)
{
    glRotatef(camera->rotation.x, 1.0, 0.0, 0.0);
    glRotatef(camera->rotation.y, 0.0, 1.0, 0.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.y += horizontal * 0.1;
    camera->rotation.x += vertical * 0.1;
}
