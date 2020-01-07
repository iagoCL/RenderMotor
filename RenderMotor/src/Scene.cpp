#include "Scene.h"

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.h"

Scene::Scene() {
    camara = new Camera();
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

void Scene::renderizarTodos() {
    for (int i = 0; i < numNodos; i++)
        nodos[i]->renderizarNodo();
}

void Scene::cambiarModel(int identificador, glm::mat4 nuevaModel) {
    mallas[identificador]->setModel(nuevaModel);
}

void Scene::incrementarColorDeLuz(int identificador, glm::vec3 incremento) {
    luces[identificador]->increaseColor(incremento);
}

void Scene::moveLuz(int identificador, glm::vec3 distance) {
    luces[identificador]->move(distance);
}

void Scene::rotateLuz(int identificador, glm::vec3 rotacion, float angulo) {
    luces[identificador]->rotate(rotacion, angulo);
}

void Scene::moverLuz(int identificador, glm::vec3 eje, float angulo) {
    if (luces[identificador]->getType() == 2)
        rotateLuz(identificador, eje, angulo);
    else if (luces[identificador]->getType() != 0)
        moveLuz(identificador, eje);
}

void Scene::incrementarColorDeTodasLasLuces(glm::vec3 incremento) {
    for (int i = 0; i < numLuces; i++)
        if (luces[i]->getType() != 0) luces[i]->increaseColor(incremento);
}

void Scene::moveTodasLasLuces(glm::vec3 distance) {
    for (int i = 0; i < numLuces; i++)
        if (luces[i]->getType() == 1 || luces[i]->getType() == 3) luces[i]->move(distance);
}

void Scene::rotateTodasLasLuces(glm::vec3 rotacion, float angulo) {
    for (int i = 0; i < numLuces; i++)
        if (luces[i]->getType() == 2 || luces[i]->getType() == 3) luces[i]->rotate(rotacion, angulo);
}

void Scene::rotateCamaraUsandoView(float angulo, glm::vec3 rotationAxis) {
    camara->viewRotation(angulo, rotationAxis);
}

void Scene::rotateCamara(float angulo, glm::vec3 rotationAxis) {
    camara->rotate(angulo, rotationAxis);
}

void Scene::moveCamara(glm::vec3 distance) {
    camara->move(distance);
}

void Scene::setProjectionDeCamara(glm::mat4 projectionMatrix_) {
    camara->setProjection(projectionMatrix_);
}

void Scene::setProjectionDeCamara(float angle, float aspectRatio, float near, float far) {
    camara->setProjection(angle, aspectRatio, near, far);
}

int Scene::getNumNodos() {
    return numNodos;
}

int Scene::getNumLuces() {
    return numLuces;
}

int Scene::getNumMallas() {
    return numMallas;
}

int Scene::getNumShaders() {
    return numShaders;
}

int Scene::addShader(Shader* nuevoShader) {
    Shader** arrayAux = new Shader*[numShaders + 1];
    for (int i = 0; i < numShaders; i++)
        arrayAux[i] = shadersCompilados[i];
    arrayAux[numShaders] = nuevoShader;
    delete shadersCompilados;
    shadersCompilados = arrayAux;
    std::cout << "\nNuevo Shader agregado en la posici�n " << numShaders;
    return numShaders++;
}

int Scene::addLight(Light* nuevaLuz) {
    Light** arrayAux = new Light*[numLuces + 1];
    for (int i = 0; i < numLuces; i++)
        arrayAux[i] = luces[i];
    arrayAux[numLuces] = nuevaLuz;
    delete luces;
    luces = arrayAux;
    std::cout << "\nNueva Luz agregada en la posici�n " << numLuces;
    return numLuces++;
}

void Scene::addModel3D(Model3D* nuevaMalla) {
    Model3D** arrayAux = new Model3D*[numMallas + 1];
    for (int i = 0; i < numMallas; i++)
        arrayAux[i] = mallas[i];
    arrayAux[numMallas] = nuevaMalla;
    delete mallas;
    mallas = arrayAux;
    std::cout << "\nNueva malla agregada en la posici�n " << numMallas;
    numMallas++;
}

void Scene::addTextureDiffuse(Texture* nuevasTexturas) {
    Texture** arrayAux = new Texture*[numMallas];
    for (int i = 0; i < (numMallas - 1); i++)
        arrayAux[i] = texturasColores[i];
    arrayAux[numMallas - 1] = nuevasTexturas;
    delete texturasColores;
    texturasColores = arrayAux;
    std::cout << "\nNueva textura de Color agregada en la posici�n " << numMallas;
}

void Scene::addTextureEmissive(Texture* nuevasTexturas) {
    Texture** arrayAux = new Texture*[numMallas];
    for (int i = 0; i < (numMallas - 1); i++)
        arrayAux[i] = texturasEmis[i];
    arrayAux[numMallas - 1] = nuevasTexturas;
    delete texturasEmis;
    texturasEmis = arrayAux;
    std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

void Scene::addTextureSpecular(Texture* nuevasTexturas) {
    Texture** arrayAux = new Texture*[numMallas];
    for (int i = 0; i < (numMallas - 1); i++)
        arrayAux[i] = texturasSpec[i];
    arrayAux[numMallas - 1] = nuevasTexturas;
    delete texturasSpec;
    texturasSpec = arrayAux;
    std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

void Scene::addTextureNormal(Texture* nuevasTexturas) {
    Texture** arrayAux = new Texture*[numMallas];
    for (int i = 0; i < (numMallas - 1); i++)
        arrayAux[i] = texturasNorm[i];
    arrayAux[numMallas - 1] = nuevasTexturas;
    delete texturasNorm;
    texturasNorm = arrayAux;
    std::cout << "\nNueva textura agregada en la posici�n " << numMallas;
}

int Scene::addTextureAndModel3D(Model3D* nuevaMalla, Texture* nuevaTexturaCol, Texture* nuevaTexturaEmis, Texture* nuevaTexturaSpec, Texture* nuevaTexturaNorm) {
    addModel3D(nuevaMalla);
    addTextureDiffuse(nuevaTexturaCol);
    addTextureEmissive(nuevaTexturaEmis);
    addTextureSpecular(nuevaTexturaSpec);
    addTextureNormal(nuevaTexturaNorm);
    return numMallas - 1;
}

void Scene::cambiarMalla(int identificador, Model3D* nuevaMalla) {
    mallas[identificador] = nuevaMalla;
    std::cout << "\nLa malla ha sido colocada en la posici�n " << identificador;
}

void Scene::cambiarLuz(int identificador, Light* nuevaLuz) {
    luces[identificador] = nuevaLuz;
    std::cout << "\nLa luz ha sido colocada en la posici�n " << identificador;
}

void Scene::cambiarTexturaColor(int identificador, Texture* nuevasTexturas) {
    texturasColores[identificador] = nuevasTexturas;
    std::cout << "\nLa textura de color ha sido colocada en la posici�n " << identificador;
}

void Scene::cambiarTexturaEmis(int identificador, Texture* nuevasTexturas) {
    texturasEmis[identificador] = nuevasTexturas;
    std::cout << "\nLa textura emisiva ha sido colocada en la posici�n " << identificador;
}

void Scene::cambiarTexturaSpec(int identificador, Texture* nuevasTexturas) {
    texturasSpec[identificador] = nuevasTexturas;
    std::cout << "\nLa textura especular ha sido colocada en la posici�n " << identificador;
}

void Scene::cambiarTexturaNorm(int identificador, Texture* nuevasTexturas) {
    texturasNorm[identificador] = nuevasTexturas;
    std::cout << "\nLa textura de bump mapping ha sido colocada en la posici�n " << identificador;
}

void Scene::eliminarNodo(int identificador) {
    Node** arrayAux = new Node*[numNodos - 1];
    for (int i = 0; i < identificador; i++)
        arrayAux[i] = nodos[i];
    for (int i = identificador + 1; i < numNodos; i++)
        arrayAux[i - 1] = nodos[i];
    numNodos--;
    delete nodos;
    nodos = arrayAux;
    std::cout << "\nNodo en la posici�n " << identificador << "eliminado; nuevo total de nodos: " << numNodos << "\n";
}

int Scene::addNode(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader, int numeroDeTexturaColor, int numeroDeTexturaEmis, int numeroDeTexturaSpec, int numeroDeTexturaNorm) {
    Node** arrayAux = new Node*[numNodos + 1];
    for (int i = 0; i < numNodos; i++)
        arrayAux[i] = nodos[i];
    arrayAux[numNodos] = new Node(camara, cantidadDeMallas, &mallas[numeroDeMalla], cantidadDeLuces, &luces[numeroDeluz], shadersCompilados[numeroDeShader], &(texturasColores[numeroDeTexturaColor]), &(texturasEmis[numeroDeTexturaEmis]), &(texturasSpec[numeroDeTexturaSpec]), &(texturasNorm[numeroDeTexturaNorm]));
    //delete[] nodos; // no se porque da error
    delete nodos;
    nodos = arrayAux;
    std::cout << "\nNuevo nodo agregado en la posici�n " << numNodos << "\n";
    return numNodos++;
}

int Scene::addNode(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader) {
    return addNode(cantidadDeMallas, numeroDeMalla, cantidadDeLuces, numeroDeluz, numeroDeShader, numeroDeMalla, numeroDeMalla, numeroDeMalla, numeroDeMalla);
}

Scene::~Scene() {
    delete camara;
    std::cout << "\nCamara Borada";
    for (int i = 0; i < numMallas; i++) {
        if (mallas[i]) delete mallas[i];
        std::cout << "\nMalla destruida: " << i;
        if (texturasColores[i]) delete texturasColores[i];
        std::cout << "\nTextura de color destruida: " << i;
        if (texturasEmis[i]) delete texturasEmis[i];
        std::cout << "\nTextura emisiva destruida: " << i;
        if (texturasSpec[i]) delete texturasSpec[i];
        std::cout << "\nTextura especular destruida: " << i;
        if (texturasNorm[i]) delete texturasNorm[i];
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

    for (int i = 0; i < numShaders; i++) {
        if (shadersCompilados[i]) delete shadersCompilados[i];
        std::cout << "\nSe ha borrado el Shader: " << i;
    }
    delete shadersCompilados;
    std::cout << "\n\nSe han borrado los shaders";

    for (int i = 0; i < numNodos; i++) {
        if (nodos[i]) delete nodos[i];
        std::cout << "\nSe ha borrado el nodo: " << i;
    }
    delete nodos;
    std::cout << "\n\nSe han borrado los nodos\n\nSe ha borrado la escena";
}