// Readfile definitions 
#pragma once
#include "stdafx.h"
#include "Scene.h"
#include <memory>

class ReadFile{
    public:
        static void matransform (stack<mat4> &transfstack, float * values) ;
        static void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
        static bool readvals (std::stringstream &s, const int numvals, float * values) ;
        static std::unique_ptr<Scene> readfile (const char * filename);
};

