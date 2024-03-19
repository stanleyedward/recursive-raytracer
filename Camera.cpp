#include "Camera.h"


Camera::Camera(vec3 eye, vec3 lookat, vec3 up, float fox){
    this->eye = eye;
    this->lookat = lookat;
    this->frontDirection = glm::normalize(eye - lookat);
    this->horDirection = glm::normalize(glm::cross(up, frontDirection));
    this->up = glm::normalize(glm::cross(frontDirection, horDirection));
    this->fov = fov;

    fovy = fov;
}

Ray Camera::createRay(float i, float j, int width, int height, float aspectRatio){
    float iF = i + 0.5f;
    float jF = j + 0.5f;

    float halfW = (float)width / 2.0f;
    float halfF = (float)height / 2.0f;
}