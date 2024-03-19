#include "Renderer.h"

Renderer::Renderer(){}

void Renderer::SceneRendering(std::string filename){

}

vec3 Renderer::ComputeLight(vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse, vec3 myspecular, float myshininess) {

	float nDotL = dot(normal, direction);

	vec3 lambert = mydiffuse * lightcolor * std::max(nDotL, 0.0f);

	float nDotH = dot(normal, halfvec);
	vec3 phong = myspecular * lightcolor * pow(std::max(nDotH, 0.0f), myshininess);

	vec3 retval = lambert + phong;
	return retval;
}

IntersectionData Renderer::GetIntersectionData(Ray ray){
    IntersectionData data = IntersectionData();
    Object* current_obj;
    float t = INFINITY;
    vec3 normal;
    vec3 point;
    float temp_t = INFINITY;

    for(int i = 0; i < m_Scene->numobjects; i++){
        current_obj = m_Scene->objects[i];
        if(current_obj->GetIntersection(ray, temp_t, normal, point)){
            if(temp_t > 0 && temp_t < t){
                t = temp_t;
                data = IntersectionData(current_obj, point, normal, t);
            }
        }
    }
    return data;
}

bool Renderer::IsBlocked(Ray ray){
    float t = INFINITY;
    vec3 normal;
    vec3 point;
    
    for(int i = 0; i < m_Scene->numobjects; i++){
        Object* current_obj = m_Scene->objects[i];
        if(current_obj->GetIntersection(ray, t, normal, point)){
            return true;
        }
    }
    return false;
}