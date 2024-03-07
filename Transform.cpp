// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1. 
  mat3 Identity = glm::mat3(1.0);

	mat3 Axis_Axis_T = mat3(
		axis.x * axis.x, axis.x * axis.y, axis.x * axis.z,
		axis.y * axis.x, axis.y * axis.y, axis.y * axis.z,
		axis.z * axis.x, axis.z * axis.y, axis.z * axis.z
		);

	mat3 A_star = mat3(
		0., axis.z, -axis.y, 
		-axis.z, 0., axis.x,
		 axis.y, -axis.x, 0.
		 );
	
	// float radians = degrees * (pi/180);
	float radians = glm::radians(degrees);
	float sin_angle = sin(radians);
	float cos_angle = cos(radians);

	mat3 Rotation = glm::mat3(cos_angle*Identity + (1.f-cos_angle)*Axis_Axis_T + sin_angle*A_star);

	return Rotation; 
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
  mat3 rotation = rotate(degrees, up);
	eye = rotation * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.  
  vec3 rotation_axis = glm::normalize(glm::cross(eye, up));
	mat3 rotation = rotate(degrees, rotation_axis);
	eye = rotation * eye;
	up = glm::normalize(glm::cross(rotation_axis, eye));
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);

	return mat4(
		u.x, v.x, w.x, 0., 
		u.y, v.y, w.y, 0., 
		u.z, v.z, w.z, 0., 
		-glm::dot(u, eye), -glm::dot(v, eye), -glm::dot(w, eye), 1.); 

}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 perspective_matrix;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  
  float theta = fovy/2.0;
  float radians = glm::radians(theta);

  float d = cos(radians)/ sin(radians);

  float A = -((zFar+zNear)/(zFar - zNear));
  float B = -((2*zFar*zNear)/(zFar-zNear));


 perspective_matrix = glm::mat4(
  d/aspect, 0.0, 0.0, 0.0,
  0.0, d, 0.0, 0.0,
  0.0, 0.0, A, B,
  0.0, 0.0, -1.0, 0.0
 ); perspective_matrix = glm::transpose(perspective_matrix);

  return perspective_matrix; 
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 scaling_matrix;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 
  scaling_matrix = mat4(
    sx, 0.0, 0.0, 0.0,
    0.0, sy, 0.0, 0.0,
    0.0, 0.0, sz, 0.0,
    0.0, 0.0, 0.0, 1.0
  ); scaling_matrix = glm::transpose(scaling_matrix);

  return scaling_matrix;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 translation_matrix;
  // YOUR CODE FOR HW2 HERE
  // Implement translation 
  translation_matrix = mat4(
    1.0, 0.0, 0.0, tx,
    0.0, 1.0, 0.0, ty,
    0.0, 0.0, 1.0, tz,
    0.0, 0.0, 0.0, 1.0
  );translation_matrix = glm::transpose(translation_matrix);

  return translation_matrix;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
