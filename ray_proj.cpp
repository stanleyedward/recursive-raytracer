#include "ray_proj.h"
using namespace glm;

Ray::Ray(float alpha, float beta, glm::vec3 p0, glm::vec3 p1){
    this->alpha = alpha;
    this->beta = beta;
    this->p0 = p0;
    this->p1 = p1;
}