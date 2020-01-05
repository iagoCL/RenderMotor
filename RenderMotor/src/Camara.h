#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camara
{
private:
	glm::mat4 view;
	glm::mat4 proj;

public:
	Camara();
	Camara(glm::mat4 nuevaView, glm::mat4 nuevaProj);
	void rotarPorView(float angulo, glm::vec3 ejeDeRotacion);
	void rotar(float angulo, glm::vec3 ejeDeRotacion);
	void desplazar(glm::vec3 desplazamiento);
	void setProj(glm::mat4 nuevaProj);
	void setProj(float angulo,float aspectRatio,float znear, float zfar);
	glm::mat4 getView();
	glm::mat4 getProj();
	~Camara();
};

