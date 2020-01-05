#pragma once
#include <glm/glm.hpp>

class Luz
{
private:
	int tipo;//0 - no hay luz; 1 - Puntual; 2 - direccional; 3 - focal
	glm::vec3 LDir;
	glm::vec3 LPos;
	glm::vec3 LCol;
	float angle;

public:
	Luz(glm::vec3 Pos, glm::vec3 Col, glm::vec3 Dir, float angulo=0.18f);//constructor focal
	Luz(glm::vec3 PosODir, glm::vec3 Col,int NulaPuntDirOFoc);//constructor direccional o puntual 
	Luz(glm::vec3 Pos, glm::vec3 Col);//contructor puntual
	Luz();//constructor a nula
	~Luz();
	void incrementarColor(glm::vec3 incremento);
	void desplazar(glm::vec3 desplazamiento);
	void rotar(glm::vec3 rotacion,float angulo);
	void enviarPosicionAlShader(glm::mat4 view, unsigned int identificadorPos, unsigned int identificadorDir = -1, unsigned int identificadorAng = -1);
	void enviarDireccionAlShader(glm::mat4 view, unsigned int identificador);
	void enviarAnguloAlShader(unsigned int identificador);
	void enviarLugarAlShader(glm::mat4 view, unsigned int identificador);
	void enviarColorAlShader(unsigned int identificador);
	int getTipo();
};

