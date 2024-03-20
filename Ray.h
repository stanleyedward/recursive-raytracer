#pragma once
#include <glm/glm.hpp>

using namespace glm;
class Ray {
	public:
		vec3 origin;
		vec3 direction;
		float t;

	public:
		Ray(vec3 origin, vec3 direction) {
			this->origin = origin;
			this->direction = normalize(direction);
		}

		void transformRay(const mat4& matrix) {
			glm::vec4 transformed_origin = matrix * glm::vec4(origin, 1.0f);
			glm::vec4 transformed_direction = matrix * glm::vec4(direction, 0.0f);

			this->origin = glm::vec3(transformed_origin);
			this->direction = glm::vec3(transformed_direction);
		}
};
