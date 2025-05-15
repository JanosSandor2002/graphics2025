#include "Camera.h"
#include <math.h>

#define DEG2RAD(x) ((x)*3.14159265f/180.0f)

void Camera_Init(Camera* cam, float x, float y, float z) {
    cam->posX = x;
    cam->posY = y;
    cam->posZ = z;
    cam->yaw = -90.0f;  // alapból nézünk az -Z irányba
    cam->pitch = 0.0f;
}

void Camera_Move(Camera* cam, float forward, float right, float up) {
    float radYaw = DEG2RAD(cam->yaw);
    float radPitch = DEG2RAD(cam->pitch);

    // előre hátra mozgás XY síkon a yaw alapján (Y felfelé)
    cam->posX += forward * cosf(radYaw);
    cam->posZ += forward * sinf(radYaw);

    // oldalirányú mozgás (jobbra balra)
    cam->posX += right * cosf(radYaw + 3.14159265f / 2);
    cam->posZ += right * sinf(radYaw + 3.14159265f / 2);

    // függőleges mozgás
    cam->posY += up;
}

void Camera_Rotate(Camera* cam, float deltaYaw, float deltaPitch) {
    cam->yaw += deltaYaw;
    cam->pitch += deltaPitch;

    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;
}

void Camera_ApplyView(const Camera* cam) {
    float radYaw = DEG2RAD(cam->yaw);
    float radPitch = DEG2RAD(cam->pitch);

    float dirX = cosf(radYaw) * cosf(radPitch);
    float dirY = sinf(radPitch);
    float dirZ = sinf(radYaw) * cosf(radPitch);

    gluLookAt(
        cam->posX, cam->posY, cam->posZ,
        cam->posX + dirX, cam->posY + dirY, cam->posZ + dirZ,
        0.0f, 1.0f, 0.0f
    );
}
