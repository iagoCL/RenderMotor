#pragma once
#include <glm/glm.hpp>

class Interpolacion
{
private:
	float t;
	float M_PI;
	float radio;

public:
	Interpolacion();
	Interpolacion(float FRadio);
	~Interpolacion();
	void incrementar(float velocidad);
	glm::vec3 bezier();
	glm::vec3 splinesCubicos();
};

