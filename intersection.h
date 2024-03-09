#ifndef intersection_h
#define intersection_h
#include <stdio.h>
#include "ray_proj.h"
#include <glm/glm.hpp>
using namespace glm;

class intersection{
    public:
        float t;
        glm::vec3 P, N;
        int object_index;
        Ray* ray;
        intersection(float t, glm::vec3 P, glm::vec3 N, int object_index, Ray* ray);
};
#endif 