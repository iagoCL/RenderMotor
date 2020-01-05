#include "Camara.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camara::Camara()
{
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f))*glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(60.0f)/*angulo de apertura*/, 1.0f/*aspect ratio*/, 0.1f/*near*/, 100.0f/*far*/);//creamos la matriz de proyeccion con una funcion predefinida
}

Camara::Camara(glm::mat4 nuevaView, glm::mat4 nuevaProj)
{
	view = nuevaView;
	proj = nuevaProj;
}

void Camara::rotarPorView(float angulo, glm::vec3 ejeDeRotacion)
{
	view=glm::rotate(glm::mat4(1.0f), angulo, /*up*/glm::vec3(view * glm::vec4(ejeDeRotacion, 0.0f)))*view;
}
void Camara::rotar(float angulo, glm::vec3 ejeDeRotacion)
{
	view = glm::rotate(glm::mat4(1.0f), angulo, /*up*/ejeDeRotacion)*view;
}

void Camara::desplazar(glm::vec3 desplazamiento)
{
	view = glm::translate(glm::mat4(1.0f), desplazamiento)*view;
}

void Camara::setProj(glm::mat4 nuevaProj)
{
	proj = nuevaProj;
}


void Camara::setProj(float angulo, float aspectRatio, float near, float far)
{
	proj = glm::perspective(glm::radians(angulo)/*angulo de apertura*/, aspectRatio/*aspect ratio*/, near/*near*/, far/*far*/);//creamos la matriz de proyeccion con una funcion predefinida
}

glm::mat4 Camara::getView()
{
	return view;
}

glm::mat4 Camara::getProj()
{
	return proj;
}

Camara::~Camara()
{
}
