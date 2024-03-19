#pragma once
#include "Scene.h"
#include "Camera.h"
#include "readfile.h"
#include "Ray.h"
#include "stdafx.h"
#include <FreeImage.h>
#include <memory>

struct IntersectionData{
    public:
        Object* intersectedObj;
        vec3 point;
        vec3 normal;
        float t;
        bool valid;

    public:
        IntersectionData(){
            valid = false;
        }
        IntersectionData(Object* intersectedObj, vec3 point, vec3 normal, float t) : intersectedObj(intersectedObj), point(point), normal(normal), t(t){
            valid = true;
        }
        ~IntersectionData(){

        }
};

class Renderer{
    public:
    Renderer();
    void SceneRendering(std::string filename);
    vec3 ComputeLight(vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse, vec3 myspecular, vec3 myshininess);
    IntersectionData GetIntersectionData(Ray ray);
    void color(IntersectionData* intData, Ray ray, BYTE* color, float* specularIn);
    vec3 ComputeColor(Ray& ray, int current_depth);
    bool IsBlocked(Ray ray);
    std::unique_ptr<Scene> m_Scene;
};