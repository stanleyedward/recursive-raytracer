#pragma once
#include "stdafx.h"
#include "Ray.h"

using namespace glm;

class Camera{
    private:
        float fovx;
        float fovy;

    public:
        Camera(){};

        vec3 up, frontDirection, horDirection, eye, lookat;
        float fov;

        Camera(vec3 eye, vec3 lookat, vec3 up, float fov);
        void SetupFovx(float aspectRatio);
        Ray createRay(float i, float j, int width, int height, float aspectRatio);
};