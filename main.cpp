/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include "ray_proj.h"
#include "intersection.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <FreeImage.h>
#include <algorithm>
const float pi = 3.14159265

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
 

void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n" 
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n" 
    << "press 'i' to run image grader test cases\n"
    << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"       
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press ESC to quit.\n" ;      
}

Ray project_ray(int i, int j){
  float new_i = i + 0.5;
  float new_j = j + 0.5;

  glm::vec3 w = glm::normalize(eye - center);
  glm::vec3 u = glm::normalize(glm::cross(up, w));
  glm::vec3 v = glm::normalize(glm::cross(w, u));

  float fovy_radian = fovy * pi/180;
  float alpha = tan(fovy_radian/2) * aspect * ((new_j - (float)w/2)/(w/2));
  float beta = tan(fovy_radian/2) * (((double)h/2 - new_i) / (h/2));

  glm::vec3 p1 = glm::normalize(alpha*u + beta*v - w);

  return Ray(alpha, beta, eye, p1);
}

bool is_point_in_triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P){
  glm::vec3 u = B-A;
  glm::vec3 v = C-A;
  glm::vec3 w = P-A;

  glm::vec3 v_cross_w = cross(v, w);
  glm::vec3 v_cross_u = cross(v, u);

  if(dot(v_cross_w, v_cross_u) < 0){ //check if r < 0
    return false;
  }

  glm::vec3 u_cross_w = cross(u, w);
  glm::vec3 u_cross_v = cross(u, v);

  if (dot(u_cross_w, u_cross_v) < 0){ //check if t < 0
    return false; 
  }

  float denominator = length(u_cross_v);
  float r = length(v_cross_w) / denominator;
  float t = length(u_cross_w) / denominator;

  return (r + t <= 1);
}

intersection intersect(Ray ray){
  glm::vec3 normal;
  glm::vec3 p_hit;
  int index_hit = -1;

  float min_t = INFINITY;
  float MIN_T_ALLOWED = 0.00001;

  object* hit_object = NULL;

  for(int i=0; i<numobjects; i++){
    object* obj = &(objects[i]);
    if(obj->type == triangle){

      glm::vec3 AT = glm::vec3(obj->transform * glm::vec4(vertex[(obj->indices)[0]], 1));
      glm::vec3 BT = glm::vec3(obj->transform * glm::vec4(vertex[(obj->indices)[1]],1));
      glm::vec3 CT = glm::vec3(obj->transform * glm::vec4(vertex[(obj->indices)[2]],1));
      
      glm::vec3 abT = BT-AT;
      glm::vec3 acT = CT-AT;
      
      glm::vec3 normalC = glm::normalize(cross(abT, acT));
      float t = (dot(AT, normalC) - dot(ray.p0, normalC))/(dot(ray.p1, normalC));
      glm::vec3 p = ray.p0 + t*ray.p1;

      if (is_point_in_triangle(AT, BT, CT, p)){
        if(min_t > t && t>MIN_T_ALLOWED){
          min_t = t;
          hit_object = obj;
          index_hit = 1;
          p_hit = p;
          normal = normalC;
      
        }
      }

    } 
    else if(obj->type == sphere){

    }
  }
}


int main(int argc, char* argv[]) {

}
