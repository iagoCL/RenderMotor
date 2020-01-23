#include <gl/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.h"
#include "IlluminationSet.h"
#include "Interpolation.h"
#include "Light.h"
#include "Model3D.h"
#include "Node.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

int main(int argc, char** argv) {
    Control::showControls();
    Scene::initOpenGL(argc, argv, "Render example");
    Camera* camera = new Camera();
    camera->setProjection(60.0f, 1.0f, 0.1f, 100.0f);
    Scene* scene = new Scene(camera);

    Model3D* cubeMesh = Model3D::getCubeModel();
    cubeMesh->initObject();

    Control* control = scene->getControl();

    auto interpolationFunction = [](const float actualTime, Interpolation* interpolation) {
        float angle = 11.0f * actualTime;
        if (angle > (2.0f * glm::pi<float>())) {
            angle -= floorf(angle / (2.f * glm::pi<float>())) * angle;
        }
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));  //Rotates the cube
        return glm::translate(glm::mat4(1.0f), interpolation->getPosition(actualTime)) * model;
    };
    auto cubeVectorRotationUpdate = [](const float actualTime) {
        float angle = 10.5f * actualTime;
        if (angle > (2.0f * glm::pi<float>())) {
            angle -= floorf(angle / (2.f * glm::pi<float>())) * angle;
        }
        glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)), angle, glm::vec3(0.0f, 1.0f, 0.0f));  //Translate the cube
        angle = 4.5f * actualTime;
        if (angle > (2.0f * glm::pi<float>())) {
            angle -= floorf(angle / (2.f * glm::pi<float>())) * angle;
        }
        return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * model;  //Rotates the cube
    };
    auto cubeCenterRotationUpdate = [](const float actualTime) {
        float angle = 5 * actualTime;
        if (angle > (2.0f * glm::pi<float>())) {
            angle -= floorf(angle / (2.f * glm::pi<float>())) * angle;
        }
        return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));  //Rotates the cube
    };

    Shader* simpleShader = new Shader("../shaders/shader.v0.vert", "../shaders/shader.v0.frag", scene);
    IlluminationSet* pointLightSet = new IlluminationSet(simpleShader);
    Light* basicPointLight = new Light(glm::vec3(0.0f, 0.0f, 11.0f), glm::vec3(0.7f));
    pointLightSet->addLight(basicPointLight);
    control->addLight(basicPointLight);
    Material* simpleMaterial = new SimpleMaterial(pointLightSet);
    Animation* vectorRotationAnimation = new Animation(4.5f, cubeVectorRotationUpdate);
    Node* simpleNode = new NodeAnimated(cubeMesh, simpleMaterial, vectorRotationAnimation);

    Texture* diffuseCubeTexture = new Texture("../img/color2.png");
    diffuseCubeTexture->applyAnisotropicFilter();
    Texture* normCubeTexture = new Texture("../img/normal.png");
    normCubeTexture->applyAnisotropicFilter();
    Texture* emissiveCubeTexture = new Texture("../img/emissive.png");
    emissiveCubeTexture->applyAnisotropicFilter();
    Texture* specularCubeTexture = new Texture("../img/specMap.png");
    specularCubeTexture->applyAnisotropicFilter();

    Shader* bumpShader = new Shader("../shaders/shader.v3.vert", "../shaders/shader.v3.frag", scene);
    IlluminationSet* bumpLightSet = new IlluminationSet(bumpShader);
    Light* redFocalLight = new Light(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0), 0.08f);
    Light* pointLight = new Light(glm::vec3(-7.0, 0.0, 3.0), glm::vec3(0.8f));
    Light* directionalLight = new Light(glm::vec3(0.0, -1.0, -1.0), glm::vec3(0.5f), 2);
    bumpLightSet->addLight(redFocalLight);
    bumpLightSet->addLight(pointLight);
    bumpLightSet->addLight(directionalLight);
    control->addLight(redFocalLight);
    control->addLight(pointLight);
    control->addLight(directionalLight);
    Material* bumpMaterial = new BumpMaterial(bumpLightSet, diffuseCubeTexture, emissiveCubeTexture, specularCubeTexture, normCubeTexture);
    Animation* centerRotationAnimation = new Animation(1.5f, cubeCenterRotationUpdate);
    Node* bumpNode = new NodeAnimated(cubeMesh, bumpMaterial, centerRotationAnimation);
    /**/
    Shader* diffuseShader = new Shader("../shaders/shader.v2.vert", "../shaders/shader.v2.frag", scene);
    IlluminationSet* diffuseMultiColorLightSet = new IlluminationSet(diffuseShader);
    Light* blueFocalLight = new Light(glm::vec3(-3.0, 3.0, 3.0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0, -3.0, -3.0), 0.5f);
    Light* redPointLight = new Light(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Light* greenDirectionalLight = new Light(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 2);
    diffuseMultiColorLightSet->addLight(blueFocalLight);
    diffuseMultiColorLightSet->addLight(redPointLight);
    diffuseMultiColorLightSet->addLight(greenDirectionalLight);
    Material* cubeDiffuseMaterial = new BumpMaterial(diffuseMultiColorLightSet, diffuseCubeTexture, emissiveCubeTexture, specularCubeTexture, nullptr);

    Interpolation* bezierInterpolation = new BezierInterpolation(3.0f, BezierInterpolation::getCirclePoints(4.5f));
    Animation* interpolationBezierAnimation = new AnimationWithInterpolation(3.0f, interpolationFunction, bezierInterpolation);
    Node* diffuseNode2 = new NodeAnimated(cubeMesh, cubeDiffuseMaterial, interpolationBezierAnimation);

    Interpolation* splinesInterpolation = new SplinesInterpolation(5.0f, SplinesInterpolation::getCirclePoints(2.0f), SplinesInterpolation::getCircleTangents());
    Animation* interpolationSplinesAnimation = new AnimationWithInterpolation(5.0f, interpolationFunction, splinesInterpolation);
    Node* diffuseNode3 = new NodeAnimated(cubeMesh, cubeDiffuseMaterial, interpolationSplinesAnimation);

    Texture* blackTexture = new Texture("../img/black.png");
    blackTexture->applyAnisotropicFilter();
    IlluminationSet* diffuseNormalLightSet = new IlluminationSet(diffuseShader);
    diffuseNormalLightSet->addLight(redFocalLight);
    diffuseNormalLightSet->addLight(pointLight);
    diffuseNormalLightSet->addLight(directionalLight);
    const char* texturesPaths[] = {
        "../img/character/maps/bodyDiffuseMap.jpg",
        "../img/character/maps/faceDiffuseMap.jpg",
        "../img/character/maps/feetDiffuseMap.jpg",
        "../img/character/maps/handsDiffuseMap.jpg",
        "../img/character/maps/legsDiffuseMap.jpg"};
    int numMallas = 0;
    Model3D** assimpModel = Model3D::loadFromFile(numMallas, "../img/character/preacherCharacterB.obj");
    for (int i = 0; i < numMallas; i++) {
        assimpModel[i]->removeColors();
        assimpModel[i]->removeColors();
        assimpModel[i]->removeTangents();
        assimpModel[i]->combineSimilarVertices();
        assimpModel[i]->recomputeNormalsAndTangents(true);
        assimpModel[i]->initObject();
        Texture* modelTexture = new Texture(texturesPaths[i]);
        modelTexture->applyAnisotropicFilter();
        Material* tmpMaterial = new BumpMaterial(diffuseNormalLightSet, modelTexture, blackTexture, blackTexture, nullptr);
        Node* tmpNode = new Node(assimpModel[i], tmpMaterial);
    }
    scene->start();

    return 0;
}