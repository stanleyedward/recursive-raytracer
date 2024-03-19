#include "Renderer.h"

Renderer::Renderer(){}


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

vec3 Renderer::ComputeColor(Ray& ray, int current_depth){
    const float eps = 1e-6f;
    float numerical_epsilon = 1e-4f;
	float L = 1;

	IntersectionData intData = GetIntersectionData(ray);
	if (!intData.valid){
		return vec3(0.0f, 0.0f, 0.0f);
    }

	vec3 color = vec3(intData.intersectedObj->ambient[0], intData.intersectedObj->ambient[1], intData.intersectedObj->ambient[2])
		+ vec3(intData.intersectedObj->emission[0], intData.intersectedObj->emission[1], intData.intersectedObj->emission[2]);



	vec3 intersection_position;
	for (int i = 0; i < m_Scene->numused; i++)
	{
		
		float V = 1;
		vec3 to_light_direction;
		vec3 fixed_to_light_direction;
		vec3 light_position = vec3(m_Scene->lights[i].x, m_Scene->lights[i].y, m_Scene->lights[i].z);
		vec3 light_color = vec3(m_Scene->lights[i].r, m_Scene->lights[i].g, m_Scene->lights[i].b);
		vec3 light_dir;
		
		
		if (m_Scene->lights[i].type == LightType::point) {
			light_dir = normalize(intData.point - light_position);
			

			to_light_direction = normalize(light_position - intData.point);
			intersection_position = intData.point + (to_light_direction * numerical_epsilon);
;

			float dirLength = length(light_position - intData.point);
			L = 1 / (m_Scene->attenuation.x + (m_Scene->attenuation.y * dirLength) + (m_Scene->attenuation.z * dirLength * dirLength));

			
			Ray ray_from_light = Ray(intersection_position, to_light_direction);
			IntersectionData intData_ToLightRay = GetIntersectionData(ray_from_light);
			if (intData_ToLightRay.valid) {
				if (intData_ToLightRay.t < dirLength)
					V = 0;
			}

		}
		else {
			V = 1;
			to_light_direction = normalize(light_position);
			light_dir = normalize(light_position);

			intersection_position = intData.point + (to_light_direction * numerical_epsilon);
			Ray ray_to_light = Ray(intersection_position, light_dir);
			IntersectionData intData_ToLightRay = GetIntersectionData(ray_to_light);
			if (intData_ToLightRay.valid)
				V = 0;

			
		}
	
		vec3 eyedir = normalize(ray.origin - intData.point);
		vec3 half0 = normalize(to_light_direction + normalize(-ray.direction));

		vec3 col0 = ComputeLight(to_light_direction, vec3(light_color), normalize(intData.normal), half0,
			vec3(intData.intersectedObj->diffuse[0], intData.intersectedObj->diffuse[1], intData.intersectedObj->diffuse[2]),
			vec3(intData.intersectedObj->specular[0], intData.intersectedObj->specular[1], intData.intersectedObj->specular[2]),
			intData.intersectedObj->shininess);
		color += V * L * col0;
	}
	vec3 eyedir = normalize(ray.origin - intData.point);
	current_depth++;
	if (current_depth <= m_Scene->maxdepth) {
		vec3 spec = vec3(intData.intersectedObj->specular[0], intData.intersectedObj->specular[1], intData.intersectedObj->specular[2]);
		if (!(spec.x == 0.0f && spec.y == 0.0f && spec.z == 0.0f)) {

			vec3 reflectionDirection = normalize(((2.0f * dot(-ray.direction, intData.normal)) * intData.normal + ray.direction));
			vec3 reflection_point_temp = intData.point + (reflectionDirection * numerical_epsilon);

			Ray reflected_ray(reflection_point_temp, reflectionDirection);
			vec3 reflected_Color = ComputeColor(reflected_ray, current_depth);

			color += spec * reflected_Color;
		}
	}
	return color;
}

void Renderer::SceneRendering(std::string filename){
    FreeImage_Initialise();

    m_Scene = ReadFile::readfile(filename.c_str());

    int temp;
    const int pixelNum = m_Scene->width * m_Scene->height;
    BYTE* pixels = new BYTE[3 * pixelNum];

    for(int i=0; i < m_Scene->height; i++){
        for(int j=0; j < m_Scene->width; j++){
            Ray ray = m_Scene->mainCamera.createRay(i, j, m_Scene->width, m_Scene->height, m_Scene->aspectRatio);
            vec3 color = ComputeColor(ray, 0);

            int slot = 3 * ((m_Scene->height - i -1) * m_Scene->width + j);
            *(pixels + 2 + slot) = static_cast<unsigned int>(std::min(255 * color.r, 255.0f));
            *(pixels + 1 + slot) = static_cast<unsigned int>(std::min(255 * color.g, 255.0f));
			*(pixels + 0 + slot) = static_cast<unsigned int>(std::min(255 * color.b, 255.0f));

        }
        temp = i;
        int counter = temp % 10;
        if (counter == 0) std::cout << "Tracing pixel row: " << i << "\n";
    }

    FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, m_Scene->width, m_Scene->height, m_Scene->width * 3, 24, 0xFF0000, 0xFF0000, 0xFF0000, false);
    FreeImage_Save(FIF_PNG, img, m_Scene->output_filename.c_str(), 0);

    std::cout << "\n***************" << "\n";
	std::cout << "Ray Tracing Complete" << "\n";
	std::cout << "\n**************" << "\n";
	FreeImage_DeInitialise();
}