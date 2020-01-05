#include "textura.h"
#include "auxiliar.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <iostream>
#include <windows.h>

int Textura::numeroTotal = 0;

Textura::Textura(unsigned int identificadorP, int numeroP) {
	identificador = identificadorP;
	numero = numeroP;
}

Textura::Textura(const char *fileName, bool anisotropico)
{
	numero = numeroTotal;
	numeroTotal++;
	identificador = loadTex(fileName, anisotropico);
}

Textura::~Textura()
{
	glDeleteTextures(1, &identificador);
	std::cout << "\nBorrado de textura: eliminacion de OpenGL hecha";
}

unsigned int Textura::loadTex(const char *fileName, bool anisotropico) {//cargamos la informacion de la textura jpg en memoria

	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map)//comprobamos si a cargado la imagen bien
	{
		
		std::cout << "Error cargando el fichero: " 	<< fileName << std::endl;

		char a;
		std::cin >> a;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);//generamos el espacio de la grafica para la textura
	glBindTexture(GL_TEXTURE_2D, texId);//activamos la textura que modificaremos
	//cargamos la textura en la grafica
	glTexImage2D(GL_TEXTURE_2D, 0/*level, numero de bitmap al que subimos los grandes, el 0 es el más alto en la jerarquía*/,
		GL_RGBA8/*como se almacenan en la gráfica*/, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);

	delete[] map;//elimina la textura de memoria principal para ahorar espacio

	glGenerateMipmap(GL_TEXTURE_2D);//genera en la gráfica todos los mipmaps de la textura generada automáticamente

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//filtro cuando dos fragmentos coinciden por hacer un muestreo insuficiente
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	if(anisotropico)
		aplicarAnisotropico();
	return texId;
}

void Textura::enviarAlShader(unsigned int identificadorUniforme) {
//	std::cout << "\n" << numero;
	glActiveTexture(GL_TEXTURE0 + numero);
	glBindTexture(GL_TEXTURE_2D, identificador);
	glUniform1i(identificadorUniforme, numero);
}

void Textura::aplicarAnisotropico()
{
	int num = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num);
	bool found = false;
	while (0 < --num & !found) {
		if (strcmp(((char *)glGetStringi(GL_EXTENSIONS, num - 1)), "GL_EXT_texture_filter_anisotropic"))
			found = true;
		//std::cout << glGetStringi( GL_EXTENSIONS, num - 1 ) << std::endl;
	}
	if (found) {
		float MaximoSoportado;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaximoSoportado);
		std::cout << "\n Textura "<< numero <<": aplicando filtro anisotrópico x" << MaximoSoportado;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaximoSoportado);
	}
	else
		std::cout << "\n Textura " << numero << ": filtro anisotrópico no soportado";
}

Textura* Textura::clone()
{
	return new Textura(identificador,numero);
}
