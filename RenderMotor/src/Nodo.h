#pragma once
#include "Malla.h"
#include "Luz.h"
#include "ShaderCompilado.h"
#include "Textura.h"
#include "Camara.h"
#include <iostream>
class Nodo
{
private:
	Camara* camara;

public:
	Malla** malla;
	Luz** luz;
	ShaderCompilado* shaderCompilado;
	Textura** texturaColor;
	Textura** texturaEmi;
	Textura** texturaSpec;
	Textura** texturaNorm;
	int numMallas;
	int numLuces;

	//nodo(glm::mat4 view1, glm::mat4 proj1, int numeroMallas, malla * Malla1[], int numeroLuces, luz * Luz1[], shaderCompilado * shaderCompilado1, textura * textura1[] = nullptr, textura * textura2[] = nullptr, textura * textura3[] = nullptr, textura * textura4[] = nullptr);
	Nodo(Camara* cam, int numeroMallas, Malla* Malla1[], int numeroLuces, Luz* Luz1[], ShaderCompilado* shaderCompilado1, Textura* textColor[] = nullptr, Textura* textEmissi[] = nullptr, Textura* textSpec[] = nullptr, Textura* textNorm[] = nullptr);

	~Nodo();

	void renderizarNodo();
};
