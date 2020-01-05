#include "Nodo.h"

#include "Malla.h"
#include "Luz.h"
#include "ShaderCompilado.h"
#include "Textura.h"
#include "Camara.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include "iostream"

/*
//Contructor alternativo no usado
nodo::nodo(glm::mat4 view1, glm::mat4 proj1, int numeroMallas, malla* Malla1[], int numerLuces, luz* Luz1[], shaderCompilado* shaderCompilado1, textura* textura1[], textura* textura2[], textura* textura3[], textura * textura4[])
{
	Malla = Malla1;

	Camara = new camara(view1, proj1);

	Luz = Luz1;
	ShaderCompilado = shaderCompilado1;
	TexturaColor = textura1;
	TexturaEmi = textura2;
	TexturaSpec = textura3;
	TexturaNorm = textura4;
	numMallas = numeroMallas;
	numLuces = numerLuces;
	for(int i=0;i<numeroMallas;i++)
		Malla[i]->initObj(ShaderCompilado->getinPos(), ShaderCompilado->getinColor(), ShaderCompilado->getinNormal(), ShaderCompilado->getinTexCoord(), ShaderCompilado->getinTangent());
	
}
*/
Nodo::Nodo(Camara* cam, int numeroMallas, Malla* malla1[],int numeroLuces, Luz* luz1[], ShaderCompilado* shaderCompilado1, Textura* textura1[], Textura* textura2[], Textura* textura3[],Textura* textura4[])
{
	malla = malla1;
	camara = cam;

	luz = luz1;
	shaderCompilado = shaderCompilado1;
	texturaColor = textura1;
	texturaEmi = textura2;
	texturaSpec = textura3;
	texturaNorm = textura4;
	numMallas = numeroMallas;
	numLuces = numeroLuces;
	glUseProgram(shaderCompilado->getprogram());//indica que incie y use el programa
	for (int i = 0; i<numeroMallas; i++)
		malla[i]->initObj(shaderCompilado->getinPos(), shaderCompilado->getinColor(), shaderCompilado->getinNormal(), shaderCompilado->getinTexCoord(), shaderCompilado->getinTangent());
}

Nodo::~Nodo()
{
	
}

void Nodo::renderizarNodo()
{
	glUseProgram(shaderCompilado->getprogram());//indica que incie y use el programa
	//Envio las luces, puede que otro nodo subiera las sullas
	for (int i = 0; i < numLuces; i++) {
		luz[i]->enviarPosicionAlShader(camara->getView(), shaderCompilado->getuLPos(i), shaderCompilado->getuLDir(i), shaderCompilado->getuLAngle(i));
		luz[i]->enviarColorAlShader(shaderCompilado->getuLCol(i));
	}
	for (int i = 0; i < numMallas; i++) {
		malla[i]->renderizarMalla(camara->getView(), camara->getProj(), shaderCompilado->getuModelViewMat(), shaderCompilado->getuModelViewProjMat(), shaderCompilado->getuNormalMat());
		if (shaderCompilado->getuColorTex() != -1)
			texturaColor[i]->enviarAlShader(shaderCompilado->getuColorTex());
		if (shaderCompilado->getuEmiTex() != -1)
			texturaEmi[i]->enviarAlShader(shaderCompilado->getuEmiTex());
		if (shaderCompilado->getuSpecTex() != -1)
			texturaSpec[i]->enviarAlShader(shaderCompilado->getuSpecTex());
		if (shaderCompilado->getuNormTex() != -1)
			texturaNorm[i]->enviarAlShader(shaderCompilado->getuNormTex());
		glBindVertexArray(malla[i]->getVAO());
		glDrawElements(GL_TRIANGLES/*son triangulos*/,/*numero de elementos*/ malla[i]->getNumCaras() * 3, GL_UNSIGNED_INT, (void*)0);//indica como interpretar el buffer de vertices
	
	}
}




