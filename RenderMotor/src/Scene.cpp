#include "Scene.h"

#include <gl/glew.h>
#include <windows.h>
#include <iostream>

#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

Scene* Scene::currentScene = nullptr;

Scene::Scene(Camera* camera_)
    : camera(camera_),
      control(new Control(this)),
      framesUpdateFPS(static_cast<unsigned int>(8e8)) {
    currentScene = this;
}

void Scene::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto itShader = shaders.begin(); itShader != shaders.end(); ++itShader) {
        (*itShader)->renderShader(view);
    }
    glutSwapBuffers();
    ++renderedFrames;
}
void Scene::updateScene() {
    const auto start = std::chrono::high_resolution_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(start - lastRenderStart).count();
    lastRenderStart = start;
    const float updateTime = static_cast<float>(timeDiff) * 1e-9f;
    timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(start - framesRenderedStart).count();
    if (timeDiff > framesUpdateFPS) {
        std::cout << "Rendered " << renderedFrames << " frames in " << timeDiff << " microseconds; FPS= " << 1e9f * static_cast<float>(renderedFrames) / static_cast<float>(timeDiff) << std::endl;
        framesRenderedStart = start;
        renderedFrames = 0;
    }

    view = camera->getView();
    const glm::mat4 proj = camera->getProj();
    for (auto itNode = nodes.begin(); itNode != nodes.end(); ++itNode) {
        (*itNode)->updateNode(updateTime, view, proj);
    }
    glutPostRedisplay();
}

void Scene::addShader(Shader* shader_) {
    shaders.push_back(shader_);
}
void Scene::addNode(Node* node_) {
    nodes.push_back(node_);
}
Camera* Scene::getCamera() const {
    return camera;
}
Control* Scene::getControl() const {
    return control;
}

Scene::~Scene() {
}

void Scene::initOpenGL(const int argc, char** argv, const char* windowTitle, const int windowSizeX, const int windowSizeY, const int windowPositionX, const int windowPositionY) {
    initContext(argc, argv, windowTitle, windowSizeX, windowSizeY, windowPositionX, windowPositionY);
    initOGL(true, true, true, glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
}
void Scene::start() {
    renderedFrames = 0;
    lastRenderStart = framesRenderedStart = std::chrono::high_resolution_clock::now();
    glutMainLoop();
}

void Scene::initContext(int argc, char** argv, const char* windowTitle, const int windowSizeX, const int windowSizeY, const int windowPositionX, const int windowPositionY) {
    std::locale::global(std::locale("spanish"));
    glutInit(&argc, argv);                      //Creates the context using GLUT
    glutInitContextVersion(3, 3);               //OpenGL version
    glutInitContextProfile(GLUT_CORE_PROFILE);  //Only core functionality

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);  //Buffer Options(Double,RGBA,Depth test)
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutInitWindowPosition(windowPositionX, windowPositionY);
    glutCreateWindow(windowTitle);

    //Start GLEW and extension
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error starting GLEW: " << glewGetErrorString(err) << std::endl;
        exit(-1);
    }
    const GLubyte* oglVersion = glGetString(GL_VERSION);
    std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;  //Shows the actual OpenGL version

    //Link different functions with GLUT
    glutReshapeFunc(Scene::resizeFuncCall);
    glutDisplayFunc(Scene::renderSceneCall);
    glutIdleFunc(Scene::updateSceneCall);
    glutKeyboardFunc(Scene::keyboardFuncCall);
    glutMouseFunc(Scene::mouseFuncCall);
    glutMotionFunc(Scene::mouseMotionFuncCall);
}

void Scene::resizeFunc(int width, int height) {
    glViewport(0, 0, width, height);
    camera->setProjection(60.0, float(width) / float(height), 0.1f, 100.0f);
    std::cout << "Scene resized to: { x = " << width << ", " << height << " }" << std::endl;
}

void Scene::initOGL(const bool enableDepthTest, const bool useCCW, const bool enableCulling, glm::vec4 backgroundColor) {
    if (enableDepthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    //Stablish background color
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

    if (enableDepthTest) {
        glFrontFace(GL_CCW);  //Draw Counter Clock Wise faces
    } else {
        glFrontFace(GL_CW);  //Draw Clock Wise faces
    }

    //glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //Render Both faces
    if (enableCulling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Scene::renderSceneCall() {
    if (currentScene) {
        currentScene->renderScene();
    } else {
        std::cout << "ERROR: rendering scene; current scene null.";
    }
}
void Scene::updateSceneCall() {
    if (currentScene) {
        currentScene->updateScene();
    } else {
        std::cout << "ERROR: rendering scene; current scene null.";
    }
}
void Scene::resizeFuncCall(int width, int height) {
    if (currentScene) {
        currentScene->resizeFunc(width, height);
    } else {
        std::cout << "ERROR: resizeing scene; current scene null.";
    }
}
void Scene::keyboardFuncCall(unsigned char key, int x, int y) {
    if (currentScene && currentScene->control) {
        currentScene->control->keyboardFunc(key, x, y);
    } else {
        std::cout << "ERROR: hadeling keyboard; current scene or control null.";
    }
}
void Scene::mouseFuncCall(int button, int state, int x, int y) {
    if (currentScene && currentScene->control) {
        currentScene->control->mouseFunc(button, state, x, y);
    } else {
        std::cout << "ERROR: hadeling mouse function; current scene or control null.";
    }
}
void Scene::mouseMotionFuncCall(int x, int y) {
    if (currentScene && currentScene->control) {
        currentScene->control->mouseMotionFunc(x, y);
    } else {
        std::cout << "ERROR: hadeling mouse movement; current scene or control null.";
    }
}