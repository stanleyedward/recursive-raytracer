#include "Camera.h"

void Camera::SetupFovx(float aspectRatio) {
	fovx = tan(radians(fovy) / 2.0f) * aspectRatio;
}

Camera::Camera(vec3 eye, vec3 lookat, vec3 up, float fov) {
	this->eye = eye;
	this->lookat = lookat;
	this->frontDirection = glm::normalize(eye - lookat);
	this->horDirection = glm::normalize(glm::cross(up, frontDirection));
	this->up = glm::normalize(glm::cross(frontDirection, horDirection));
	this->fov = fov;
	fovy = fov;
}

Ray Camera::createRay(float i, float j, int width, int height, float aspectRatio) {
	float iF = i + 0.5f; //vertical
	float jF = j + 0.5f; //horizotal

	float halfW = (float)width / 2.0f;
	float halfH = (float)height / 2.0f;

	float alpha = fovx * ((jF - halfW) / halfW);
	float beta = tan(radians(fovy) / 2) * ((halfH - iF) / halfH);

	vec3 rayDirection = glm::normalize(-frontDirection + horDirection * alpha  + up * beta);
	Ray ray(eye, rayDirection);

	return ray;
}
