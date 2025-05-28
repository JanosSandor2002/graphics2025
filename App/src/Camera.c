#include "Camera.h"
#include <math.h>
#include <GL/glu.h>

#define DEG2RAD(x) ((x)*3.14159265f/180.0f) //makro for switching

void Camera_Init(Camera* cam, float x, float y, float z) {
    cam->posX = x;
    cam->posY = y;
    cam->posZ = z;
    cam->yaw = -90.0f; //around y
    cam->pitch = 0.0f; //around x
}

void Camera_Move(Camera* cam, float forward, float right, float up) {
    float radYaw = DEG2RAD(cam->yaw);
    float radPitch = DEG2RAD(cam->pitch);

    cam->posX += forward * cosf(radYaw);
    cam->posZ += forward * sinf(radYaw);

    cam->posX += right * cosf(radYaw + 3.14159265f / 2);
    cam->posZ += right * sinf(radYaw + 3.14159265f / 2);

    cam->posY += up;
}

void Camera_Rotate(Camera* cam, float deltaYaw, float deltaPitch) {
    cam->yaw += deltaYaw;
    cam->pitch += deltaPitch;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f; //against turning freedom loss
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;
}

void Camera_ApplyView(const Camera* cam) {
    float radYaw = DEG2RAD(cam->yaw);
    float radPitch = DEG2RAD(cam->pitch);

    float dirX = cosf(radYaw) * cosf(radPitch); //identity matrix
    float dirY = sinf(radPitch);
    float dirZ = sinf(radYaw) * cosf(radPitch);

    gluLookAt(
        cam->posX, cam->posY, cam->posZ, //eye position
        cam->posX + dirX, cam->posY + dirY, cam->posZ + dirZ, //look direction
        0.0f, 1.0f, 0.0f
    );
}
