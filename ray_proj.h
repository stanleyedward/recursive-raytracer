#ifndef ray_proj_h
#define ray_proj_h

#include <stdio.h>
#include <glm/glm.hpp>
using namespace glm;

class Ray{
    public:
        glm::vec3 p0, p1;
        float alpha, beta;
        Ray(float alpha, float beta, glm::vec3 p0, glm::vec3 p1);
};

#endif