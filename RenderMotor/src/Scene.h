#ifndef RENDER_MOTOR_SCENE_H
#define RENDER_MOTOR_SCENE_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Node.h"

class Scene {
   private:
    Camera* camara;
    Node** nodos;
    Light** luces;
    Model3D** mallas;
    Shader** shadersCompilados;
    Texture** texturasColores;
    Texture** texturasEmis;
    Texture** texturasSpec;
    Texture** texturasNorm;
    int numNodos;
    int numLuces;
    int numMallas;
    int numShaders;

    void addModel3D(Model3D* nuevaMalla);
    void addTextureDiffuse(Texture* nuevasTexturas);
    void addTextureEmissive(Texture* nuevasTexturas);
    void addTextureSpecular(Texture* nuevasTexturas);
    void addTextureNormal(Texture* nuevasTexturas);

   public:
    Scene();

    void renderizarTodos();

    void cambiarModel(int identificador, glm::mat4 nuevaModel);

    void incrementarColorDeLuz(int identificador, glm::vec3 incremento);
    void moveLuz(int identificador, glm::vec3 distance);
    void rotateLuz(int identificador, glm::vec3 rotacion, float angulo);

    void moverLuz(int identificador, glm::vec3 rotacion, float angulo = 0.1f);

    void incrementarColorDeTodasLasLuces(glm::vec3 incremento);
    void moveTodasLasLuces(glm::vec3 distance);
    void rotateTodasLasLuces(glm::vec3 rotacion, float angulo);

    void rotateCamaraUsandoView(float angulo, glm::vec3 rotationAxis);
    void rotateCamara(float angulo, glm::vec3 rotationAxis);
    void moveCamara(glm::vec3 distance);
    void setProjectionDeCamara(glm::mat4 projectionMatrix_);
    void setProjectionDeCamara(float angulo, float aspectRatio, float znear, float zfar);

    int getNumNodos();
    int getNumLuces();
    int getNumMallas();
    int getNumShaders();

    int addShader(Shader* nuevoShader);
    int addLight(Light* nuevaLuz);
    int addTextureAndModel3D(Model3D* nuevaMalla, Texture* nuevaTexturaCol = nullptr, Texture* nuevaTexturaEmis = nullptr, Texture* nuevaTexturaSpec = nullptr, Texture* nuevaTexturaNorm = nullptr);

    void cambiarMalla(int identificador, Model3D* nuevaMalla);
    void cambiarLuz(int identificador, Light* nuevaLuz);
    void cambiarTexturaColor(int identificador, Texture* nuevasTexturas);
    void cambiarTexturaEmis(int identificador, Texture* nuevasTexturas);
    void cambiarTexturaSpec(int identificador, Texture* nuevasTexturas);
    void cambiarTexturaNorm(int identificador, Texture* nuevasTexturas);

    void eliminarNodo(int identificador);

    int addNode(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader, int numeroDeTexturaColor, int numeroDeTexturaEmis, int numeroDeTexturaSpec, int numeroDeTexturaNorm);
    int addNode(int cantidadDeMallas, int numeroDeMalla, int cantidadDeLuces, int numeroDeluz, int numeroDeShader);

    ~Scene();
};
#endif //RENDER_MOTOR_SCENE_H