#include "Luz.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/gl.h>

#include <iostream>

Luz::Luz(glm::vec3 Pos, glm::vec3 Col, glm::vec3 Dir, float angulo)
{
	LPos = Pos;
	LCol = glm::clamp(Col, glm::vec3(0.0f), glm::vec3(1.0f));
	LDir = glm::normalize(Dir);
	angle = angulo;
	tipo = 3;
}

Luz::Luz(glm::vec3 PosODir, glm::vec3 Col, int NulaPuntDirOFoc)
{
	tipo = NulaPuntDirOFoc;
	if (NulaPuntDirOFoc == 1) {
		LPos = PosODir;
		LDir = glm::vec3(0.0f);
	}
	else {
		LDir = glm::normalize(PosODir);
		LPos = glm::vec3(0.0f);
	}
	if (tipo == 3) angle = 0.18f;
	else angle = 0;

	LCol = LCol = glm::clamp(Col, glm::vec3(0.0f), glm::vec3(1.0f));
}

Luz::Luz(glm::vec3 Pos, glm::vec3 Col){
	LPos = Pos;
	LCol = glm::clamp(Col, glm::vec3(0.0f), glm::vec3(1.0f));
	LDir = glm::vec3(0.0f);
	angle = 0.0f;
	tipo = 1;
}

Luz::Luz()
{
	tipo = 0;
	angle = 0.0f;
	LPos = glm::vec3(0.0f);
	LDir = glm::vec3(0.0f);
	LCol = glm::vec3(0.0f);
}


Luz::~Luz()
{
}

void Luz::incrementarColor(glm::vec3 incremento) {
	LCol += incremento;
	LCol = glm::clamp(LCol, glm::vec3(0.0f), glm::vec3(1.0f));
}

void Luz::desplazar(glm::vec3 desplazamiento) {
	LPos += desplazamiento;
}

void Luz::rotar(glm::vec3 rotacion, float angulo)
{
	LDir = glm::vec3(glm::rotate(glm::mat4(1.0f), angulo, rotacion)*glm::vec4(LDir, 0.0f));
}

void Luz::enviarPosicionAlShader(glm::mat4 view, unsigned int identificadorPos, unsigned int identificadorDir, unsigned int identificadorAng) {
	if (tipo == 0) std::cout << "Aviso: se esta enviando la posición de una luz de tipo nulo";
	else {
		if (tipo != 2 && identificadorPos != -1)	enviarLugarAlShader(view, identificadorPos);
		if (tipo != 1 && identificadorDir != -1)	enviarDireccionAlShader(view, identificadorDir);
		if (tipo == 3 && identificadorAng != -1)	enviarAnguloAlShader(identificadorAng);
	}
}

void Luz::enviarDireccionAlShader(glm::mat4 view, unsigned int identificador)
{
	glm::vec3 temp = glm::normalize(glm::vec3(view *  glm::vec4(LDir, 0.0f)));
	glUniform3fv(identificador, 1, &(temp[0]));
}

void Luz::enviarAnguloAlShader(unsigned int identificador)
{
	if (identificador != -1) glUniform1f(identificador, angle);
}

void Luz::enviarLugarAlShader(glm::mat4 view, unsigned int identificador)
{
	glm::vec3 temp = glm::vec3(view *  glm::vec4(LPos, 1.0f));
	glUniform3fv(identificador, 1, &(temp[0]));
}



void Luz::enviarColorAlShader(unsigned int identificador) {
	if (identificador != -1) glUniform3fv(identificador, 1, &(LCol[0]));
}

int Luz::getTipo()
{
	return tipo;
}