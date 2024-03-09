#include "intersection.h"
#include "ray_proj.h"

intersection::intersection(float t, glm::vec3 P, glm::vec3 N, int object_index, Ray* ray){
    this->t = t;
    this->P = P;
    this->N = N;
    this->object_index = object_index;
    this->ray = ray;
}