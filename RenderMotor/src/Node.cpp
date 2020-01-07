#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Model3D.h"
#include "Shader.h"
#include "Texture.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include "iostream"

Node::Node(Camera* cam, int numeroMallas, Model3D* malla1[], int numeroLuces, Light* luz1[], Shader* shaderCompilado1, Texture* textura1[], Texture* textura2[], Texture* textura3[], Texture* textura4[]) {
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
    glUseProgram(shaderCompilado->getprogram());  //indica que incie y use el programa
    for (int i = 0; i < numeroMallas; i++) {
        malla[i]->linkShaderVBOs(shaderCompilado->getinPos(), shaderCompilado->getinColor(), shaderCompilado->getinNormal(), shaderCompilado->getinTexCoord(), shaderCompilado->getinTangent());
    }
}

Node::~Node() {
}

void Node::renderizarNodo() {
    glUseProgram(shaderCompilado->getprogram());  //indica que incie y use el programa
    //Envio las luces, puede que otro nodo subiera las sullas
    for (int i = 0; i < numLuces; i++) {
        luz[i]->sendToShader(camara->getView(), shaderCompilado->getuLPos(i), shaderCompilado->getuLDir(i), shaderCompilado->getuLAngle(i));
        luz[i]->sendColorToShader(shaderCompilado->getuLCol(i));
    }
    for (int i = 0; i < numMallas; i++) {
        if (shaderCompilado->getuColorTex() != -1) {
            texturaColor[i]->sendToShaderProgram(shaderCompilado->getuColorTex());
        }
        if (shaderCompilado->getuEmiTex() != -1) {
            texturaEmi[i]->sendToShaderProgram(shaderCompilado->getuEmiTex());
        }
        if (shaderCompilado->getuSpecTex() != -1) {
            texturaSpec[i]->sendToShaderProgram(shaderCompilado->getuSpecTex());
        }
        if (shaderCompilado->getuNormTex() != -1) {
            texturaNorm[i]->sendToShaderProgram(shaderCompilado->getuNormTex());
        }
        malla[i]->renderModel(camara->getView(), camara->getProj(), shaderCompilado->getuModelViewMat(), shaderCompilado->getuModelViewProjMat(), shaderCompilado->getuNormalMat());
    }
}