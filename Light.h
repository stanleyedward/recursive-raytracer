#pragma once

enum class LightType { directional, point };

class Light{
public:
	float x, y, z;
	float r, g, b;
	LightType type;

public:
	Light() = default;
	Light(float x, float y, float z, float r, float g, float b, LightType type)
		 : x(x), y(y), z(z), r(r), g(g), b(b), type(type) {}
};
