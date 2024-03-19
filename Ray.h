#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Ray{
    public:
        vec3 origin;
        vec3 direction;
        float t;

    public:
        Ray(vec3 origin, vec3 direction){
            this->origin = origin;
            this->direction = normalize(direction);
        }

        void transformRay(const mat4& matrix){
            vec4 transformed_origin = matrix * vec4(origin, 1.0f);
            vec4 transformed_direction = matrix * vec4(direction, 0.0f);

            this->origin = vec3(transformed_origin);
            this->direction = vec3(transformed_direction);
        }
};