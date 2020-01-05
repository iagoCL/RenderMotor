#include "Database.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/gl.h>

#include "ShaderCompilado.h"

#include "iostream"

#include "BOX.h"

Database::Database()
{
	camara = new Camara();
	nodos = nullptr;
	luces = nullptr;
	mallas = nullptr;
	shadersCompilados = nullptr;
	texturasColores = nullptr;
	texturasEmis = nullptr;
	texturasSpec = nullptr;
	texturasNorm = nullptr;
	numNodos = 0;
	numLuces = 0;
	numMallas = 0;
	numShaders = 0;
	

}



void Database::renderizarTodos()
{
	for (int i = 0; i < numNodos; i++)
		nodos[i]->renderizarNodo();
}

void Database::cambiarModel(int identificador, glm::mat4 nuevaModel)
{
	mallas[identificador]->setModel(nuevaModel);
}


void Database::incrementarColorDeLuz(int identificador, glm::vec3 incremento)
{
	luces[identificador]->incrementarColor(incremento);
}

void Database::desplazarLuz(int identificador, glm::vec3 desplazamiento)
{
	luces[identificador]->desplazar(desplazamiento);
}

void Database::rotarLuz(int identificador, glm::vec3 rotacion, float angulo)
{
	luces[identificador]->rotar(rotacion, angulo);
}

void Database::moverLuz(int identificador, glm::vec3 eje, float angulo)
{
	if (luces[identificador]->getTipo() == 2)
		rotarLuz(identificador, eje, angulo);
	else if (luces[identificador]->getTipo() != 0)
		desplazarLuz(identificador, eje);
}

void Database::incrementarColorDeTodasLasLuces(glm::vec3 incremento)
{
	for (int i = 0; i < numLuces; i++)
		if (luces[i]->getTipo() != 0) luces[i]->incrementarColor(incremento);
}

void Database::desplazarTodasLasLuces(glm::vec3 desplazamiento)
{
	for (int i = 0; i < numLuces; i++)
		if (luces[i]->getTipo() == 1 || luces[i]->getTipo() == 3) luces[i]->desplazar(desplazamiento);
}

void Database::rotarTodasLasLuces(glm::vec3 rotacion, float angulo)
{
	for (int i = 0; i < numLuces; i++)
		if (luces[i]->getTipo() == 2 || luces[i]->getTipo() == 3) luces[i]->rotar(rotacion, angulo);
}

void Database::rotarCamaraUsandoView(float angulo, glm::vec3 ejeDeRotacion)
{
	camara->rotarPorView(angulo, ejeDeRotacion);
}

void Database::rotarCamara(float angulo, glm::vec3 ejeDeRotacion)
{
	camara->rotar(angulo, ejeDeRotacion);
}

void Database::desplazarCamara(glm::vec3 desplazamiento)
{
	camara->desplazar(desplazamiento);
}

void Database::setProjDeCamara(glm::mat4 nuevaProj)
{
	camara->setProj(nuevaProj);
}

void Database::setProjDeCamara(float angulo, float aspectRatio, float znear, float zfar)
{
	camara->setProj( angulo,  aspectRatio,  znear,  zfar);
}

int Database::getNumNodos()
{
	return numNodos;
}

int Database::getNumLuces()
{
	return numLuces;
}

int Database::getNumMallas()
{
	return numMallas;
}

int Database::getNumShaders()
{
	return numShaders;
}

int Database::a�adirShader(ShaderCompilado * nuevoShader)
{
	ShaderCompilado** arrayAux = new ShaderCompilado*[numShaders+1];
	for (int i = 0; i < numShaders; i++)
		arrayAux[i] = shadersCompilados[i];
	arrayAux[numShaders] = nuevoShader;
	delete shadersCompilados;
	shadersCompilados = arrayAux;
	std::cout << "\nNuevo Shader agregado en la posici�n " << numShaders;
	return numShaders++;
}

int Database::a�adirLuz(Luz * nuevaLuz)
{
	Luz** arrayAux = new Luz*[numLuces + 1];
	for (int i = 0; i < numLuces; i++)
		arrayAux[i] = luces[i];
	arrayAux[numLuces] = nuevaLuz;
	delete luces; 
	luces = arrayAux;
	std::cout << "\nNueva Luz agregada en la posici�n " << numLuces;
	return numLuces++;
}

void Database::a�adirMalla(Malla * nuevaMalla)
{
	Malla** arrayAux = new Malla*[numMallas + 1];
	for (int i = 0; i < numMallas; i++)
		arrayAux[i] = mallas[i];
	arrayAux[numMallas] = nuevaMalla;
	delete mallas;
	mallas = arrayAux;
	std::cout << "\nNueva malla agregada en la posici�n " << numMallas;
	numMallas++;
}

void Database::a�adirTexturaColor(Textura * nuevasTexturas)
{
	Textura** arrayAux = new Textura*[numMallas];
	for (int i = 0; i < (numMallas-1); i++)
		arrayAux[i] = texturasColores[i];
	arrayAux[numMallas - 1] = nuevasTexturas;
	delete texturasColores; 
	texturasColores = arrayAux;
	std::cout << "\nNueva textura de Color agregada en la posici�n " << numMallas;
}

void Database::a�adirTexturaEmis(Textura * nuevasTexturas)
{
	Textura** arrayAux = new Textura*[numMallas];
	for (int i = 0; i < (numMallas - 1); i++)
		arrayAux[i] = texturasEmis[i];
	arrayAux[numMallas - 1] = nuevasTexturas;
	delete texturasEmis;
	texturasEmis = arrayAux;
	std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

void Database::a�adirTexturaSpec(Textura * nuevasTexturas)
{
	Textura** arrayAux = new Textura*[numMallas];
	for (int i = 0; i < (numMallas - 1); i++)
		arrayAux[i] = texturasSpec[i];
	arrayAux[numMallas - 1] = nuevasTexturas;
	delete texturasSpec; 
	texturasSpec = arrayAux;
	std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

void Database::a�adirTexturaNorm(Textura * nuevasTexturas)
{
	Textura** arrayAux = new Textura*[numMallas];
	for (int i = 0; i < (numMallas - 1); i++)
		arrayAux[i] = texturasNorm[i];
	arrayAux[numMallas - 1] = nuevasTexturas;
	delete texturasNorm; 
	texturasNorm = arrayAux;
	std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

int Database::a�adirMallaYTextura(Malla * nuevaMalla, Textura * nuevaTexturaCol, Textura * nuevaTexturaEmis, Textura * nuevaTexturaSpec, Textura * nuevaTexturaNorm)
{
	a�adirMalla(nuevaMalla);
	a�adirTexturaColor(nuevaTexturaCol);
	a�adirTexturaEmis(nuevaTexturaEmis);
	a�adirTexturaSpec(nuevaTexturaSpec);
	a�adirTexturaNorm(nuevaTexturaNorm);
	return numMallas - 1;
}

void Database::cambiarMalla(int identificador, Malla * nuevaMalla)
{
	mallas[identificador] = nuevaMalla;
	std::cout << "\nLa malla ha sido colocada en la posici�n " << identificador;
}

void Database::cambiarLuz(int identificador, Luz * nuevaLuz)
{
	luces[identificador] = nuevaLuz;
	std::cout << "\nLa luz ha sido colocada en la posici�n " << identificador;
}

void Database::cambiarTexturaColor(int identificador, Textura * nuevasTexturas)
{
	texturasColores[identificador] = nuevasTexturas;
	std::cout << "\nLa textura de color ha sido colocada en la posici�n " << identificador;
}

void Database::cambiarTexturaEmis(int identificador, Textura * nuevasTexturas)
{
	texturasEmis[identificador] = nuevasTexturas;
	std::cout << "\nLa textura emisiva ha sido colocada en la posici�n " << identificador;
}

void Database::cambiarTexturaSpec(int identificador, Textura * nuevasTexturas)
{
	texturasSpec[identificador] = nuevasTexturas;
	std::cout << "\nLa textura especular ha sido colocada en la posici�n " << identificador;
}

void Database::cambiarTexturaNorm(int identificador, Textura * nuevasTexturas)
{
	texturasNorm[identificador] = nuevasTexturas;
	std::cout << "\nLa textura de bump mapping ha sido colocada en la posici�n " << identificador;
}

void Database::eliminarNodo(int identificador)
{
	Nodo** arrayAux = new Nodo*[numNodos - 1];
	for (int i = 0; i < identificador; i++)
		arrayAux[i] = nodos[i];
	for (int i = identificador+1; i < numNodos; i++)
		arrayAux[i-1] = nodos[i];
	numNodos--;
	delete nodos;
	nodos = arrayAux;
	std::cout << "\nNodo en la posici�n " << identificador << "eliminado; nuevo total de nodos: "<<numNodos<<"\n";
}

int Database::a�adirNodo(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader, int numeroDeTexturaColor, int numeroDeTexturaEmis, int numeroDeTexturaSpec, int numeroDeTexturaNorm)
{
	Nodo** arrayAux = new Nodo*[numNodos+1];
	for (int i = 0; i < numNodos; i++)
		arrayAux[i] = nodos[i];
	arrayAux[numNodos] = new Nodo(camara, cantidadDeMallas, &mallas[numeroDeMalla], cantidadDeLuces, &luces[numeroDeluz], shadersCompilados[numeroDeShader], &(texturasColores[numeroDeTexturaColor]), &(texturasEmis[numeroDeTexturaEmis]), &(texturasSpec[numeroDeTexturaSpec]), &(texturasNorm[numeroDeTexturaNorm]));
	//delete[] nodos; // no se porque da error
	delete nodos;
	nodos = arrayAux;
	std::cout << "\nNuevo nodo agregado en la posici�n " << numNodos<<"\n";
	return numNodos++;
}

int Database::a�adirNodo(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader)
{
	return a�adirNodo( cantidadDeMallas,  numeroDeMalla,  cantidadDeLuces,  numeroDeluz,  numeroDeShader, numeroDeMalla, numeroDeMalla, numeroDeMalla, numeroDeMalla);
}





Database::~Database()
{

	delete camara;
	std::cout << "\nCamara Borada";
	for (int i = 0; i < numMallas; i++) {
		if(mallas[i]) delete mallas[i];
		std::cout << "\nMalla destruida: " << i;
		if(texturasColores[i]) delete texturasColores[i];
		std::cout << "\nTextura de color destruida: " << i;
		if(texturasEmis[i]) delete texturasEmis[i];
		std::cout << "\nTextura emisiva destruida: " << i;
		if(texturasSpec[i]) delete texturasSpec[i];
		std::cout << "\nTextura especular destruida: " << i;
		if(texturasNorm[i]) delete texturasNorm[i];
		std::cout << "\nTextura normal destruida: " << i;
	}

	delete mallas;
	std::cout << "\n\nArray de mallas borrado";
	delete texturasColores;
	std::cout << "\nArray de texturas de color borrado, recuerde que por copias debera borarlas usted mismo";
	delete texturasEmis;
	std::cout << "\nArray de texturas emisivas borrado, recuerde que por copias debera borarlas usted mismo";
	delete texturasSpec;
	std::cout << "\nArray de texturas especulares borrado, recuerde que por copias debera borarlas usted mismo";
	delete texturasNorm;
	std::cout << "\nArray de texturas normales borrado, recuerde que por copias debera borarlas usted mismo\n";

	for (int i = 0; i < numLuces; i++) {
		if (luces[i]) delete luces[i];
		std::cout << "\nSe ha borrado la luz: " << i;
	}
	delete luces;
	std::cout << "\n\nSe han borrado las luces";

	for (int i = 0; i < numShaders; i++){
		if(shadersCompilados[i])delete shadersCompilados[i];
		std::cout << "\nSe ha borrado el Shader: " << i;
	}
	delete shadersCompilados;
	std::cout << "\n\nSe han borrado los shaders";

	for (int i = 0; i < numNodos; i++) {
		if(nodos[i]) delete nodos[i];
		std::cout << "\nSe ha borrado el nodo: " << i;
	}
	delete nodos;
	std::cout << "\n\nSe han borrado los nodos\n\nSe ha borrado la escena";
}
