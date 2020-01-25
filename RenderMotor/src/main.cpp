#include <gl/glew.h>

#define GLM_FORCE_RADIANS
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#include "Camera.h"
#include "IlluminationSet.h"
#include "Interpolation.h"
#include "Light.h"
#include "Model3D.h"
#include "Node.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

static const float PI_DOUBLE = 2.0f * glm::pi<float>();

int main(int argc, char** argv) {
    std::shared_ptr<Scene> scene;
    {
        const auto start = std::chrono::high_resolution_clock::now();
        Control::showControls();
        Scene::initOpenGL(argc, argv, "Render example");
        auto camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, -10.0f), 60.0f, 1.0f, 0.1f, 100.0f);
        scene = std::make_shared<Scene>(camera);

        std::shared_ptr<Model3D> mesh(Model3D::getCubeModel());
        mesh->initObject();

        std::shared_ptr<Control> control = scene->getControl();

        auto interpolationFunction = [](const float actualTime, Interpolation* interpolation) {
            float angle = 11.0f * actualTime;
            if (angle > PI_DOUBLE) {
                angle -= PI_DOUBLE;
            }
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 1.0f));  //Rotates the cube
            return glm::translate(glm::mat4(1.0f), interpolation->getPosition(actualTime)) * model;
        };
        auto cubeVectorRotationUpdate = [](const float actualTime) {
            float angle = 0.4f * actualTime;
            if (angle > PI_DOUBLE) {
                angle -= PI_DOUBLE;
            }
            glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)), angle, glm::vec3(0.0f, 1.0f, 0.0f));  //Translate the cube
            if (angle > PI_DOUBLE) {
                angle -= PI_DOUBLE;
            }
            return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 1.0f)) * model;  //Rotates the cube
        };
        auto cubeCenterRotationUpdate = [](const float actualTime) {
            float angle = 4.0f * actualTime;
            if (angle > PI_DOUBLE) {
                angle -= PI_DOUBLE;
            }
            return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));  //Rotates the cube
        };

        //Cube of colors
        std::shared_ptr<Shader> shader = Shader::createShaderFromFiles("../shaders/shader.v0.vert", "../shaders/shader.v0.frag", scene);
        std::shared_ptr<IlluminationSet> illuminationSet = IlluminationSet::createIlluminationSet(shader);
        auto pointLight = std::make_shared<LightPoint>(glm::vec3(0.0f, 0.0f, 11.0f), glm::vec3(0.7f));
        illuminationSet->addLight(pointLight);
        control->addLight(pointLight);
        std::shared_ptr<Material> material = SimpleMaterial::createSimpleMaterial(illuminationSet);
        auto animation = std::make_shared<Animation>(PI_DOUBLE / 0.4f, cubeVectorRotationUpdate);
        std::shared_ptr<Node> node = NodeAnimated::createNodeAnimated(mesh, material, animation);

        auto diffuseTexture = std::make_shared<Texture>("../img/color2.png");
        auto normalsTexture = std::make_shared<Texture>("../img/normal.png");
        auto emissiveTexture = std::make_shared<Texture>("../img/emissive.png");
        auto specularTexture = std::make_shared<Texture>("../img/specMap.png");
        auto blackTexture = std::make_shared<Texture>("../img/black.png");
        diffuseTexture->applyAnisotropicFilter();
        normalsTexture->applyAnisotropicFilter();
        emissiveTexture->applyAnisotropicFilter();
        specularTexture->applyAnisotropicFilter();
        blackTexture->applyAnisotropicFilter();

        //Cubes with color lights orbiting
        shader = Shader::createShaderFromFiles("../shaders/shader.v2.vert", "../shaders/shader.v2.frag", scene);
        illuminationSet = IlluminationSet::createIlluminationSet(shader);
        pointLight = std::make_shared<LightPoint>(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        auto directionalLight = std::make_shared<LightDirectional>(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f));
        auto focalLight = std::dynamic_pointer_cast<LightDirectional>(std::make_shared<LightFocal>(glm::vec3(-3.0, 3.0, 3.0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0f, -3.0f, -3.0f), 0.5f));
        illuminationSet->addLight(focalLight);
        illuminationSet->addLight(pointLight);
        illuminationSet->addLight(directionalLight);
        material = TextureMaterial::createTextureMaterial(illuminationSet, diffuseTexture, emissiveTexture, specularTexture);

        std::shared_ptr<Interpolation> interpolation = std::make_shared<BezierInterpolation>(4.0f, BezierInterpolation::getCirclePoints(4.5f));
        animation = std::make_shared<AnimationWithInterpolation>(4.0f, interpolationFunction, interpolation);
        node = NodeAnimated::createNodeAnimated(mesh, material, animation);

        interpolation = std::make_shared<SplinesInterpolation>(4.0f, SplinesInterpolation::getCirclePoints(2.0f), SplinesInterpolation::getCircleTangents());
        animation = std::make_shared<AnimationWithInterpolation>(4.0f, interpolationFunction, interpolation);
        node = NodeAnimated::createNodeAnimated(mesh, material, animation);

        //Centered cube with bump mapping
        shader = Shader::createShaderFromFiles("../shaders/shader.v3.vert", "../shaders/shader.v3.frag", scene);
        illuminationSet = IlluminationSet::createIlluminationSet(shader);
        pointLight = std::make_shared<LightPoint>(glm::vec3(-7.0, 0.0, 3.0), glm::vec3(0.8f));
        directionalLight = std::make_shared<LightDirectional>(glm::vec3(0.0, -1.0, -1.0), glm::vec3(0.5f));
        focalLight = std::dynamic_pointer_cast<LightDirectional>(std::make_shared<LightFocal>(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.08f));
        illuminationSet->addLight(focalLight);
        illuminationSet->addLight(pointLight);
        illuminationSet->addLight(directionalLight);
        control->addLight(pointLight);
        control->addLight(focalLight);
        control->addLight(directionalLight);
        material = BumpMaterial::createBumpMaterial(illuminationSet, diffuseTexture, emissiveTexture, specularTexture, normalsTexture);
        animation = std::make_shared<Animation>(glm::pi<float>(), cubeCenterRotationUpdate);
        node = NodeAnimated::createNodeAnimated(mesh, material, animation);

        //Character
        shader = Shader::createShaderFromFiles("../shaders/shader.v2.vert", "../shaders/shader.v2.frag", scene);
        illuminationSet = IlluminationSet::createIlluminationSet(shader);
        illuminationSet->addLight(focalLight);
        illuminationSet->addLight(pointLight);
        illuminationSet->addLight(directionalLight);
        std::vector<std::shared_ptr<Model3D>> assimpModels;
        Model3D::loadFromFile(assimpModels, "../img/character/preacherCharacterB.obj");
        const int numModels = assimpModels.size();
        if (numModels != 5) {
            std::cout << "WARNING: different number of meshes and textures." << std::endl;
        }
        const char* texturesPaths[5] = {
            "../img/character/maps/bodyDiffuseMap.jpg",
            "../img/character/maps/faceDiffuseMap.jpg",
            "../img/character/maps/feetDiffuseMap.jpg",
            "../img/character/maps/handsDiffuseMap.jpg",
            "../img/character/maps/legsDiffuseMap.jpg"};
        for (int i = 0; i < numModels; ++i) {
            mesh = assimpModels.at(i);
            mesh->removeColors();
            mesh->removeColors();
            mesh->removeTangents();
            mesh->combineSimilarVertices();
            mesh->recomputeNormalsAndTangents(true);
            mesh->initObject();
            diffuseTexture = std::make_shared<Texture>(texturesPaths[i]);
            diffuseTexture->applyAnisotropicFilter();
            material = TextureMaterial::createTextureMaterial(illuminationSet, diffuseTexture, blackTexture, blackTexture);
            node = Node::createNode(mesh, material);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Scene loaded in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
    }
    Scene::setCurrentScene(scene);
    return 0;
}