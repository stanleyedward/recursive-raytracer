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
const float pi = 3.14159265;

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
      glm::mat4 inverse_transform = inverse(obj->transform);
      glm::vec4 p0t = inverse_transform * glm::vec4(ray.p0, 1);
      glm::vec4 p1t = inverse_transform * glm::vec4(ray.p1, 0);

      Ray transformed_ray = Ray(ray.alpha, ray.beta, glm::vec3(p0t), glm::vec3(p1t));

      float a = dot(transformed_ray.p1, transformed_ray.p1);
      float b = 2*dot(transformed_ray.p1, (transformed_ray.p0 - obj->center));
      float c = dot(transformed_ray.p0 - obj->center, transformed_ray.p0 - obj->center) - obj->size * obj ->size;

      float discriminant = b*b - 4*a*c;

      if(discriminant < 0){
        
      }

      else{
        float root1 = (-b + sqrt(discriminant)) / (2*a);
        float root2 = (-b - sqrt(discriminant)) / (2*a);

        float t = std::min(root1, root2);

        if(min_t > t && t > MIN_T_ALLOWED){
          min_t = t;
          hit_object = obj;
          index_hit = i;

          glm::vec3 pt = transformed_ray.p0 + t*transformed_ray.p1;
          glm::vec4 p = obj->transform * glm::vec4(pt, 1);
          glm::vec4 normal_sphere = glm::vec4(pt = obj->center, 0);
          p_hit = glm::vec3(p/p.w);

          normal = normalize(vec3(transpose(inverse_transform) * normal_sphere));
        }
      }
      else if(obj->type == cube){
          //none
      }

    }
  }
  return intersection(min_t,p_hit, normal, index_hit, &ray)
}

vec4 ComputeLight(glm::vec3 direction, glm::vec4 light_color, glm::vec3 normal, glm::vec3 half_vec, glm::vec4 diffuse, glm::vec4 specular, float shininess, bool is_triangle){
    
    float nDotL = dot(normal, direction)  ;

    glm::vec4 lambertian = diffuse * light_color * std::max(nDotL, 0.0f);

    float nDotH = glm::dot(normal, half_vec);
    glm::vec4 phong = specular * light_color * pow(std::max(nDotH, 0.0f), shininess);
    
    glm::vec4 retval = lambertian + phong;

    return retval;
}

Ray* find_color(intersection* intersect_temp, BYTE* color, float* specular_input){
  if(intersect_temp->t != INFINITY){
    if(current_depth > 2){
      int ii=0;
      ii++;
    }
    object* obj = &(objects[intersect_temp->object_index]);

    glm::vec4 all_colors = glm::vec4(0, 0, 0, 0);
    glm::vec3 ray_direction = normalize(intersect_temp->ray->p1);
    glm::vec3 rec_direction = normalize(ray_direction - 2*dot(ray_direction, intersect_temp->N) *intersect_temp->N);
    
    Ray recursive_ray = Ray(0, 0, intersect_temp->P, rec_direction);

    for(int i = 0; i<numused; i++){ 
      glm:vec3 position0 = glm::vec3(lightposn[i*4 + 0], lightposn[i*4 + 1]], lightposn[i*4 +2]);
      glm::vec3 direction0;

      float L = 1;
      float V = 1;

      if(lightposn[i*4 + 3] != 0){
        direction0 = position0 = intersect_temp->P;
        float d = length(direction0);
        L = 1/(attenuation.x, attenuation.y*d, attenuation.z*d*d);

        Ray ray_to_light_source = Ray(0,0, intersect_temp->, direction0);
        intersection visibility = intersect(ray_to_light_source);

        if(visibility.t < 1){
          V = 0;
        }

        direction0 = normalize(direction0);

      }
      else{
        direction0 = normalize(direction0);
        Ray ray_to_light_source = Ray(0, 0, intersect_temp->P, direction0);
        intersection visibility = intersect(ray_to_light_source);
        if(visibility.t != INFINITY){
          V=0;
        }
      }
      glm::vec3 half0 = normalize(direction0 - ray_direction);
      glm::vec4 light_color_temp = glm::vec4(lightcolor[i*4], lightcolor[i*4+1], lightcolor[i*4+2],1); 
      glm::vec4 diffuse_temp = glm::vec4(1);
      glm::vec4 specular_temp = glm::vec4(1);

      for(int j=0; j<4; j++){
        diffuse_temp[j] = (obj->diffuse)[j];
        specular_temp[j] = (obj->specular)[j];
      }

      glm::vec4 color0 = ComputeLight(direction0, light_color_temp, intersect_temp->N, half0, diffuse_temp, specular_temp, obj->shininess, obj->type==triangle);
      all_colors += V * L * color0;
    }

    BYTE current_color[3];
    current_color[0] = ((obj->ambient)[2] + (obj->emission)[2] + all_colors[2]) * 255;
    current_color[1] = ((obj->ambient)[1] + (obj->emission)[1] + all_colors[1]) * 255;
    current_color[2] = ((obj->ambient)[0] + (obj->emission)[0] + all_colors[0]) * 255;

    if(current_depth > 1){
      for(int i=0; i< current_depth-1; i++){
        current_color[0] *= specular_input[(i)*3+2];
        current_color[1] *= specular_input[(i)*3+1];
        current_color[2] *= specular_input[(i)*3];
      }

      color[0] += current_color[0];
      color[1] += current_color[1];
      color[2] += current_color[2];
      
    }
    else{
      color[0] = ((obj->ambient)[2] + (obj->emission)[2] + all_colors) * 255;
      color[1] = ((obj->ambient)[1] + (obj->emission)[1] + all_colors) * 255;
      color[2] = ((obj->ambient)[0] + (obj->emission)[0] + all_colors) * 255;
      
    }

    return &recursive_ray;
  }
  else{
    if (current_depth>1){
      //none
    }
    else{
      color[0]=0;
      color[1]=0;
      color[2]=0;
    }

    return nullptr;
  }
}

void ray_tracer(Ray ray, BYTE* color, float* specular_input){
  current_depth++;

  intersection intersection = intersect(ray);
  if(intersection.t != INFINITY){
    object* obj = &(objects[intersection.object_index]);
    specular_input[(current_depth-1) * 3] = (obj->specular)[0];
    specular_input[(current_depth-1) * 3 + 1] = (obj->specular)[1];
    specular_input[(current_depth-1) * 3 + 2] = (obj->specular)[2];
  }

  Ray* recursive_ray = find_color(&intersection, color, specular_input);

  if(recursive_ray != nullptr && current_depth<max_depth){
    ray_tracer(*recursive_ray, color, specular_input);
  }
}

void save_screenshot(BYTE* pixels, string filename){
  FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, w, h, w* 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
  std::cout << "Saving screenshot: " << filename << "\n";
    
  FreeImage_Save(FIF_PNG, img, filename.c_str(), 0);
}

int main(int argc, char* argv[]) {
  int done_val = 0;
  max_depth = 5;
  float all_specular_lights[max_depth * 3];
  attenuation = vec3(1,0, 0);

  FreeImage_Initialise();
  string fname = readfile(argv[1]);
  int pix = w*h;
  BYTE pixels[pix* 3];
  for(int i=0; i<h;i++){
    for(int j=0; j<w; j++){
      current_depth = 0;
      Ray ray = project_ray(i, j);
      ray_tracer(ray, &pixels[(i*w+j)*3], all_specular_lights);

      int percent = (i*j / (float)pix)*100;
      if(percent%100 >=done_val){
        printf("Finished %d %%\n", percent);
        done_val+=2;
      }
    }
  }

  if(fname.empty()){
    save_screenshot(pixels, "Result.png");
  }

  save_screenshot(pixels, fname);
  printHelp();
  FreeImage_DeInitialise();

  return 0;

}
