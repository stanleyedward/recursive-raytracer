#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "Transform.h"

static enum {view, translate, scale} transop;

class Scene{
    public:
        int width = 640;
        int height = 460;
        float aspectRatio;
        vec3 attenuation = vec3(1, 0, 0);

        std::string output_filename = "output.png";

        int maxdepth = 5;
        int currentdepth = 0;

        const static int numLights = 10;
        Light* lights = new Light[numLights];
        int numused;

        Camera mainCamera;

        const static int maxobjects = 100000;
        int numobjects;
        Object* objects[maxobjects];
    
    public:
        Scene(){}
        ~Scene(){
            for(int i = 0; i < numobjects; i++){
                delete objects[i];
            }
            delete[] lights;
        }
};