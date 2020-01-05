#pragma once
#include "Camara.h"
#include "Nodo.h"
#include "Luz.h"
#include <glm/glm.hpp>

class Database
{
private:
	Camara* camara;
	Nodo** nodos;
	Luz** luces;
	Malla** mallas;
	ShaderCompilado** shadersCompilados;
	Textura** texturasColores;
	Textura** texturasEmis;
	Textura** texturasSpec;
	Textura** texturasNorm;
	int numNodos;
	int numLuces;
	int numMallas;
	int numShaders;

	void añadirMalla(Malla* nuevaMalla);
	void añadirTexturaColor(Textura* nuevasTexturas);
	void añadirTexturaEmis(Textura* nuevasTexturas);
	void añadirTexturaSpec(Textura* nuevasTexturas);
	void añadirTexturaNorm (Textura* nuevasTexturas);

public:
	Database();

	void renderizarTodos();

	void cambiarModel(int identificador, glm::mat4 nuevaModel);

	void incrementarColorDeLuz(int identificador, glm::vec3 incremento);
	void desplazarLuz(int identificador, glm::vec3 desplazamiento);
	void rotarLuz(int identificador, glm::vec3 rotacion, float angulo);

	void moverLuz(int identificador, glm::vec3 rotacion, float angulo = 0.1f);

	void incrementarColorDeTodasLasLuces(glm::vec3 incremento);
	void desplazarTodasLasLuces( glm::vec3 desplazamiento);
	void rotarTodasLasLuces( glm::vec3 rotacion, float angulo);

	void rotarCamaraUsandoView(float angulo, glm::vec3 ejeDeRotacion);
	void rotarCamara(float angulo, glm::vec3 ejeDeRotacion);
	void desplazarCamara(glm::vec3 desplazamiento);
	void setProjDeCamara(glm::mat4 nuevaProj);
	void setProjDeCamara(float angulo, float aspectRatio, float znear, float zfar);

	int getNumNodos();
	int getNumLuces();
	int getNumMallas();
	int getNumShaders();

	int añadirShader(ShaderCompilado* nuevoShader);
	int añadirLuz(Luz* nuevaLuz);
	int añadirMallaYTextura(Malla* nuevaMalla, Textura* nuevaTexturaCol = nullptr, Textura* nuevaTexturaEmis = nullptr, Textura* nuevaTexturaSpec = nullptr, Textura* nuevaTexturaNorm = nullptr);

	void cambiarMalla(int identificador, Malla* nuevaMalla);
	void cambiarLuz(int identificador, Luz* nuevaLuz);
	void cambiarTexturaColor(int identificador, Textura* nuevasTexturas);
	void cambiarTexturaEmis(int identificador, Textura* nuevasTexturas);
	void cambiarTexturaSpec(int identificador, Textura* nuevasTexturas);
	void cambiarTexturaNorm(int identificador, Textura* nuevasTexturas);

	void eliminarNodo(int identificador);

	int añadirNodo(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader, int numeroDeTexturaColor, int numeroDeTexturaEmis, int numeroDeTexturaSpec, int numeroDeTexturaNorm);
	int añadirNodo(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader);

	~Database();
};

