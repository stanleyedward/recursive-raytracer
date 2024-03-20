
#include "Scene.h"
#include "Camera.h"
#include "readfile.h"
#include "Renderer.h"


int main(int argc, char* argv[])
{
	std::string filename = argv[1];

	Renderer* renderer = new Renderer();

	std::cout << filename << std::endl;
	renderer->SceneRendering(filename);
	std::cout << "Completed: " << filename << " , output saved at: " << renderer->m_Scene->output_filename << std::endl;
	
	delete renderer;
	return 0;
}