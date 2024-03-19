/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include "readfile.h"
using namespace std;


// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void ReadFile::matransform(stack<mat4> &transfstack, float* values) 
{
  mat4 transform = transfstack.top(); 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
  vec4 newval = transform * valvec; 
  for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void ReadFile::rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool ReadFile::readvals(stringstream &s, const int numvals, float* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

std::unique_ptr<Scene> ReadFile::readfile(const char* filename)
{
    float ambient[3]={ 0,0,0 };
    float diffuse[3]={ 0,0,0 };
    float specular[3]={ 0,0,0 };
    float emission[3] = { 0,0,0 };
    float shininess;

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    //Scene* scene = new Scene();

    int maxvertices;
    int vertexCounter = 0;
    std::unique_ptr<Vertex[]> localVertices = std::make_unique <Vertex[]>(1);


    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {

                stringstream s(str);
                s >> cmd;
                int i;
                float values[10]; // Position and color for light, colors for others

                bool validinput; // Validity of input 
                if (cmd == "point" || cmd == "directional") {
                    if (scene->numused == scene->numLights) { // No more Lights 
                        cerr << "Reached Maximum Number of Lights " << scene->numused << " Will ignore further lights\n";
                    }
                    else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {
                            LightType type = LightType::point;
                            /*if (cmd == "point") {
                                type = LightType::point;
                            }
                            else */if (cmd == "directional") {
                                type = LightType::directional;
                            }
                            scene->lights[scene->numused] = Light(values[0], values[1], values[2], values[3], values[4], values[5], type);
                            scene->numused++;
                        }
                    }
                }
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxvertices = values[0];
                        localVertices = std::make_unique <Vertex[]>(maxvertices);
                    }
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        localVertices[vertexCounter] = Vertex(values[0], values[1], values[2]);
                        vertexCounter++;
                    }
                }
                else if (cmd == "tri" || cmd == "sphere")
                { 
                    if (scene->numobjects == scene->maxobjects) { // No more objects 
                        cerr << "Reached Maximum Number of Objects " << scene->numobjects << " Will ignore further objects\n";
                    }
                    else {
                        if (cmd == "sphere") {
                            validinput = readvals(s, 4, values);
                            if(validinput)
                                scene->objects[scene->numobjects] = new Sphere(vec3(values[0], values[1], values[2]), values[3]);
                        }
                        else /*if (cmd == "tri")*/ {
                            validinput = readvals(s, 3, values);
                            if (validinput)
                                scene->objects[scene->numobjects] = new Triangle(localVertices[values[0]], localVertices[values[1]], localVertices[values[2]]);
                        }
                        if (validinput) {
                            Object* obj = scene->objects[scene->numobjects];
                            for (i = 0; i < 3; i++) {
                                (obj->ambient)[i] = ambient[i];
                                (obj->diffuse)[i] = diffuse[i];
                                (obj->specular)[i] = specular[i];
                                (obj->emission)[i] = emission[i];
                                obj->shininess = shininess;
                            }

                            obj->transform = transfstack.top();
                            obj->inverseTransform = inverse(obj->transform);
                            obj->inverseTransposedTransform = inverse(transpose(obj->transform));
                            obj->transposedInverseTransform = transpose(obj->inverseTransform);
                            scene->numobjects++;
                        }
                    }
                }
                else if (cmd == "output") {
                    s >> scene->output_filename;
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        scene->maxdepth = values[0];
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        scene->attenuation = vec3(values[0], values[1], values[2]);
                    }
                }
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i];
                        }
                    }
                }
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i];
                        }
                    }
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i];
                        }
                    }
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i];
                        }
                    }
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    if (validinput) {
                        scene->width = (int)values[0]; scene->height = (int)values[1];
                    }
                }
                else if (cmd == "camera") {
                    validinput = readvals(s, 10, values);
                    if (validinput) {
                        float lookfromx = values[0];
                        float lookfromy = values[1];
                        float lookfromz = values[2];

                        float lookatx = values[3];
                        float lookaty = values[4];
                        float lookatz = values[5];

                        float upx = values[6];
                        float upy = values[7];
                        float upz = values[8];

                        float fovy = values[9];

                        scene->mainCamera = Camera(vec3(lookfromx, lookfromy, lookfromz), vec3(lookatx, lookaty, lookatz), vec3(upx, upy, upz), fovy);
                    }
                }
                else if (cmd == "translate") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        float tx = values[0];
                        float ty = values[1];
                        float tz = values[2];
                        mat4 translateMatrix = Transform::translate(tx, ty, tz);
                        rightmultiply(translateMatrix, transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        float scalex = values[0];
                        float scaley = values[1];
                        float scalez = values[2];
                        mat4 scaleMatrix = Transform::scale(scalex, scaley, scalez);
                        rightmultiply(scaleMatrix, transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        float rotatex = values[0];
                        float rotatey = values[1];
                        float rotatez = values[2];

                        float angle = values[3];
                        vec3 axis = glm::normalize(vec3(rotatex, rotatey, rotatez));
                        mat3 rotateMatrix = Transform::rotate(angle, axis);
                        mat4 rotateMat4x4(rotateMatrix); //convertion from 3x3 to 4x4
                        rightmultiply(rotateMat4x4, transfstack);
                    }
                }
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                    }
                }
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }


    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
    scene->aspectRatio = (float)scene->width / (float)scene->height;
    scene->mainCamera.SetupFovx(scene->aspectRatio);
    return std::move(scene);
}
