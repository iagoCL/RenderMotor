#ifndef RENDER_MOTOR_SCENE_H
#define RENDER_MOTOR_SCENE_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Animation.h"
#include "Camera.h"
#include "Control.h"
#include "Node.h"
#include "Shader.h"

class Shader;
class Node;
class Control;
class Scene {
   private:
    static Scene* currentScene;

    Control* control;
    Camera* camera;
    std::vector<Shader*> shaders;
    std::vector<Node*> nodes;

    glm::mat4 view;

    static void initOGL(const bool enableDepthTest, const bool useCCW, const bool enableCulling, glm::vec4 backgroundColor);
    static void initContext(int argc, char** argv, const char* windowTitle, const int windowSizeX, const int windowSizeY, const int windowPositionX, const int windowPositionY);
    void resizeFunc(int width, int height);

    static void resizeFuncCall(int width, int height);
    static void renderSceneCall();
    static void updateSceneCall();
    static void keyboardFuncCall(unsigned char key, int x, int y);
    static void mouseFuncCall(int button, int state, int x, int y);
    static void mouseMotionFuncCall(int x, int y);

   public:
    Scene(Camera* camera_ = new Camera());
    static void initOpenGL(int argc, char** argv, const char* windowTitle, const int windowSizeX = 500, const int windowSizeY = 500, const int windowPositionX = 0, const int windowPositionY = 0);
    void start();

    void renderScene() const;
    void updateScene();

    void addShader(Shader* shader_);
    void addNode(Node* node_);
    Camera* getCamera() const;
    Control* getControl() const;

    ~Scene();
};
#endif  //RENDER_MOTOR_SCENE_H