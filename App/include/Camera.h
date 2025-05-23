#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>


typedef struct {
    float posX, posY, posZ;
    float yaw, pitch;
} Camera;

void Camera_Init(Camera* cam, float x, float y, float z);
void Camera_Move(Camera* cam, float forward, float right, float up);
void Camera_Rotate(Camera* cam, float deltaYaw, float deltaPitch);
void Camera_ApplyView(const Camera* cam);

#endif
