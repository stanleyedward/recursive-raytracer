/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 
EXTERN const char * fileThename;
EXTERN int maxdepth;
EXTERN vec3 attenuation;
EXTERN int max_depth;
EXTERN int current_depth;

#ifdef MAINPROGRAM 
vec3 attenuation_init(1,0,0) ;
int max_depth_init = 5;
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
float fovy = 90.0 ; // For field of view
#else 
EXTERN vec3 eyeinit ; 
EXTERN vec3 upinit ; 
EXTERN vec3 center ; 
EXTERN int amountinit;
EXTERN int w, h ; 
EXTERN float fovy ; 
#endif 

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
// EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
// EXTERN mat4 projection, modelview; // The mvp matrices
// EXTERN GLuint projectionPos, modelviewPos; // Uniform locations of the above matrices
static enum {view, translate, scale} transop ; // which operation to transform 
enum shape {cube, sphere, teapot} ;
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
// const int numLights = 10 ; 
// // EXTERN GLfloat lightposn [4*numLights] ; // Light Positions
// // EXTERN GLfloat lightcolor[4*numLights] ; // Light Colors
// // EXTERN GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
// EXTERN int numused ;                     // How many lights are used 

// vertices
const int numVertices = 100000 ;
EXTERN vec3 vertex [numVertices] ;
EXTERN int numvertex ;

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN float ambient[4] ;
EXTERN float diffuse[4] ;
EXTERN float specular[4] ;
EXTERN float emission[4] ;
EXTERN float shininess ;

// For multiple objects, read from a file.  
const int maxobjects = 100000 ;
EXTERN int numobjects ; 
EXTERN struct object {
  shape type ; 
  float size ;
  float ambient[4] ;
  float diffuse[4] ;
  float specular[4] ;
  float emission[4] ;
  float shininess ;
  int indices[3] ;
  vec3 center;
  mat4 transform ; 
} objects[maxobjects] ;

// Variables to set uniform params for lighting fragment shader 
EXTERN uint lightcol ; 
EXTERN uint lightpos ; 
EXTERN uint numusedcol ; 
EXTERN uint enablelighting ; 
EXTERN uint ambientcol ; 
EXTERN uint diffusecol ; 
EXTERN uint specularcol ; 
EXTERN uint emissioncol ; 
EXTERN uint shininesscol ; 

