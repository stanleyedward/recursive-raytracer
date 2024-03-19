#pragma once
#include "glm/glm.hpp"
#include "Ray.h"
#include "stdafx.h"

using namespace glm;
enum class Shape {sphere, triangle};

struct Vertex{
    Vertex() = default;
    Vertex(float x, float y, float z) : x(x), y(y), z(z){}
    float x, y, z;
};

class Object{
    public:
        Object() = default;

        virtual ~Object() {

        }
        virtual bool GetIntersection(const Ray rayIn, float& out_t, vec3& out_n, vec3& out_p) = 0;
    
    public:
        Shape type;
        float ambient[3] = {0, 0, 0};
        float diffuse[3] = {0, 0, 0};
        float specular[3] = {0, 0, 0};
        float shininess = 0;

        mat4 transform, inverseTransform, inverseTransposedTransform, transposedInversedTransforrm;

};

class Triangle: public Object{
    public:
        Vertex vertices[3];
        bool GetIntersection(const Ray rayIn, float& out_t, vec3& out_n, vec3& out_p);

        Triangle(Vertex v1, Vertex v2, Vertex v3) : vertices {v1, v2, v3}{
            type = Shape::triangle;
        }

};

class Sphere: public Object{
    public:
        bool GetIntersection(const Ray rayIn, float& out_t, vec3& out_n, vec3& out_p);
        vec3 center;
        float radius;
        bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);

        Sphere(vec3 center, float radius) : center(center), radius(radius){
            type = Shape::sphere;
        }

    private:
        float GetDiscriminant(float a, float b, float c);

};