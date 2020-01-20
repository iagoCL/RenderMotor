#include <gl/glew.h>
#include <windows.h>

#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h>
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

void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

//Funciones de inicializaci�n y destrucci�n
void initContext(int argc, char** argv);
void initOGL();
void destroy();

Interpolation* cubo1;
Interpolation* cubo2;
Scene* escena;
Camera* camera;
Node* movingNodes[4];

//Utiles para mover con el raton
int xZero = 0;
int yZero = 0;
//variable que mantiene el estado actual del eje y de la c�mara (para evitar pasarse de +-90�)
float yAngle = 0;
int luzSeleccionada = 4;

int main(int argc, char** argv) {
    std::locale::global(std::locale("spanish"));

    initContext(argc, argv);
    initOGL();
    camera = new Camera();
    camera->setProjection(60.0f, 1.0f, 0.1f, 100.0f);
    escena = new Scene(camera);

    Model3D* malla = Model3D::getCubeModel();  //TODO: avoid model duplication
    malla->initObject();


    Light** lights = new Light*[7];
    lights[0] = new Light(glm::vec3(0.0f, 0.0f, 11.0f), glm::vec3(0.7f));                                               //luz basica del shader 0
    lights[1] = new Light(glm::vec3(-3.0, 3.0, 3.0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0, -3.0, -3.0), 0.5f);    //foco azul
    lights[2] = new Light(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f));                                    //luz puntual roja
    lights[3] = new Light(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 2);                                    //luz direccional verde
    lights[4] = new Light(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0), 0.08f);  //foco normal rojo
    lights[5] = new Light(glm::vec3(-7.0, 0.0, 3.0), glm::vec3(0.8f));                                                  //luz puntual normal
    lights[6] = new Light(glm::vec3(0.0, -1.0, -1.0), glm::vec3(0.5f), 2);                                              //luz direccional normal

    Shader* simpleShader = new Shader("../shaders/shader.v0.vert", "../shaders/shader.v0.frag", escena);
    IlluminationSet* pointLightSet = new IlluminationSet(simpleShader);
    pointLightSet->addLight(lights[0]);
    Material* simpleMaterial = new SimpleMaterial(pointLightSet);
    Node* simpleNode =movingNodes[1]= new Node(malla, simpleMaterial);

    Texture* diffuseCubeTexture = new Texture("../img/color2.png");
    diffuseCubeTexture->applyAnisotropicFilter();
    Texture* normCubeTexture = new Texture("../img/normal.png");
    normCubeTexture->applyAnisotropicFilter();
    Texture* emissiveCubeTexture = new Texture("../img/emissive.png");
    emissiveCubeTexture->applyAnisotropicFilter();
    Texture* specularCubeTexture = new Texture("../img/specMap.png");
    specularCubeTexture->applyAnisotropicFilter();

    Shader* bumpShader = new Shader("../shaders/shader.v3.vert", "../shaders/shader.v3.frag", escena);
    IlluminationSet* bumpLightSet = new IlluminationSet(bumpShader);
    bumpLightSet->addLight(lights[4]);
    bumpLightSet->addLight(lights[5]);
    bumpLightSet->addLight(lights[6]);
    Material* bumpMaterial = new BumpMaterial(bumpLightSet, diffuseCubeTexture, emissiveCubeTexture, specularCubeTexture, normCubeTexture);
    Node* bumpNode =movingNodes[0]= new Node(malla, bumpMaterial);
    /**/
    Shader* diffuseShader = new Shader("../shaders/shader.v2.vert", "../shaders/shader.v2.frag", escena);
    IlluminationSet* diffuseMultiColorLightSet = new IlluminationSet(diffuseShader);
    diffuseMultiColorLightSet->addLight(lights[1]);
    diffuseMultiColorLightSet->addLight(lights[2]);
    diffuseMultiColorLightSet->addLight(lights[3]);
    Material* cubeDiffuseMaterial = new BumpMaterial(diffuseMultiColorLightSet, diffuseCubeTexture, emissiveCubeTexture, specularCubeTexture, nullptr);
    Node* diffuseNode2 =movingNodes[2]= new Node(malla, cubeDiffuseMaterial);
    Node* diffuseNode3 =movingNodes[3]= new Node(malla, cubeDiffuseMaterial);

    Texture* blackTexture = new Texture("../img/black.png");
    blackTexture->applyAnisotropicFilter();
    IlluminationSet* diffuseNormalLightSet = new IlluminationSet(diffuseShader);
    diffuseNormalLightSet->addLight(lights[4]);
    diffuseNormalLightSet->addLight(lights[5]);
    diffuseNormalLightSet->addLight(lights[6]);
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

    cubo1 = new BezierInterpolation(3.0f, BezierInterpolation::getCirclePoints(4.5f));
    cubo2 = new SplinesInterpolation(5.0f, SplinesInterpolation::getCirclePoints(2.0f), SplinesInterpolation::getCircleTangents());

    //inicializamos el bucle de eventos
    glutMainLoop();

    //debe ir despues del main loop o no habra nada que pintar
    destroy();
    return 0;
}

//////////////////////////
// Funciones auxiliares
void initContext(int argc, char** argv) {
    //creamos el contexto usando GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);               //usamos opengl 3.3
                                                //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); Para usar 3.1 con opciones de 3.2
    glutInitContextProfile(GLUT_CORE_PROFILE);  //Solo usa core; no tiene retro compatibilidad

    //cramos el frame buffer y la ventana
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);  //indicamos que uaremos doble buffer, rgba, y almacenaremos la profundidad
    glutInitWindowSize(500, 500);                               //creamos la ventana; el contexto no es eficaz hasta crear la ventana
    glutInitWindowPosition(0, 0);                               //arriba a la izquierda
    glutCreateWindow("Pr�cticas GLSL");

    //inicializamos Glew y sus extensiones
    glewExperimental = GL_TRUE;  //activamos la extensiones experimentales; usa las extensiones aunque estas sean experimentales
    GLenum err = glewInit();     //inicializamos glew y vemos si hay error
    if (GLEW_OK != err)          //comprueba si hay error
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;  //muestra el mensaje de error si hay error
        exit(-1);                                                        //sale con un codigo de error
    }
    const GLubyte* oglVersion = glGetString(GL_VERSION);                              //obtiene la version de opengl que estaremos usando si no ha habido ningun error
    std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;  //muestra la version por pantalla

    //indica a Glut que funciones tratan que evento
    //Puede ir en cualquier lado antes del main loop
    glutReshapeFunc(resizeFunc);
    glutDisplayFunc(renderFunc);
    glutIdleFunc(idleFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotionFunc);
}

void initOGL() {
    //configuro el cauce

    //activo el test de profundidad
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);

    //establezco el color de fondo
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    glFrontFace(GL_CCW);  //pinta caras hacia nosotros en Counter Clock Wise
    //glFrontFace(GL_CW);//pinta caras hacia nosotros en Clock Wise; pinta la parte de atras
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //indica que se rendericen ambas caras y que los polygonos se rellenen; poner solo front esta deprecated; hay que poner front y back
    glEnable(GL_CULL_FACE);                     //activamos el culling para las caras de atras
}

//destroy
void destroy() {
    //Libera los recursos utilizados
    //Los destructores de las distintas clases van liberando su memoria
    delete escena;
    delete cubo1;
    delete cubo2;
    std::cout << "\n\nSe ha eliminado a todo.";
}

void renderFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    escena->renderScene();

    //cambia entre el buffer front y el back
    //Es una llamada bloqueante, espera a que acabe todo para llamarse
    glutSwapBuffers();
}

void resizeFunc(int width, int height) {
    glViewport(0, 0, width, height);

    camera->setProjection(60.0, float(width) / float(height), 0.1f, 100.0f);

    glutPostRedisplay();
}

void idleFunc() {
    glm::mat4 model1 = glm::mat4(1.0f);
    static float angle = 0.0f;
    angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.1f;
    model1 = glm::rotate(model1, angle, glm::vec3(1.0f, 1.0f, 0.0f));

    //model 2
    static float angle2 = 0.0f;  //rotacion
    angle2 = (angle2 > 3.141592f * 2.0f) ? 0 : angle2 + 0.03f;
    glm::mat4 model2 = glm::rotate(glm::translate(glm::mat4(1.0f), /*vector de radio*/ glm::vec3(5.0f, 0.0f, 0.0f)), angle2, glm::vec3(0.0f, 1.0f, 0.0f));  //model2=Trasladar el radio*rotate,

    static float translangle = 0.0f;  //angulo de translaci�n
    translangle = (translangle > 3.141592f * 2.0f) ? 0 : translangle + 0.005f;

    model2 = glm::rotate(glm::mat4(1.0f), translangle, glm::vec3(0.0f, 1.0f, 0.0f)) * model2;  //rota la matriz para girar el cubo

    cubo2->increaseTime(0.01f);
    cubo1->increaseTime(0.01f);

    glm::mat4 model3 = glm::translate(glm::mat4(1.0f), /**glm::vec3(2.0f)/**/ cubo1->getPosition() /**/) * model1;   //model2=Trasladar el radio*rotate,
    glm::mat4 model4 = glm::translate(glm::mat4(1.0f), /**glm::vec3(-2.0f)/**/ cubo2->getPosition() /**/) * model1;  //model2=Trasladar el radio*rotate,

    /**/
    movingNodes[0]->setModel(model1);
    movingNodes[1]->setModel(model2);
    movingNodes[2]->setModel(model3);
    movingNodes[3]->setModel(model4);  //*/
    glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
    std::cout << "Se ha pulsado la tecla " << key << std::endl
              << std::endl;
    //los glm::rotate y glm::translate se podr�an cambiar por la matriz hardcodeada, pero as� queda m�s claro
    switch (key) {
        case 'w':  //move hacia delante
            camera->move(glm::vec3(0.0f, 0.0f, 1.0f));
            glutPostRedisplay();
            break;
        case 's':  //move hacia atras
            camera->move(glm::vec3(0.0f, 0.0f, -1.0f));
            glutPostRedisplay();
            break;
        case 'a':  //move hacia la izquierda
            camera->move(glm::vec3(1.0f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case 'd':  //move hacia la derecha
            camera->move(glm::vec3(-1.0f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case 'e':  //rotate hacia la derecha
            camera->viewRotation(-0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            glutPostRedisplay();
            break;
        case 'q':  //rotate hacia la izquierda
            camera->viewRotation(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            glutPostRedisplay();
            break;
            /*
            //mover luz
        case 'o':  //mover hacia delante
            escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.0f, -1.0f), 0.05f);
            glutPostRedisplay();
            break;
        case 'l':  //mover hacia detras
            escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.0f, 1.0f), -0.05f);
            glutPostRedisplay();
            break;
        case 'i':  //mover hacia la  derecha
            escena->moverLuz(luzSeleccionada, glm::vec3(0.1f, 0.0f, 0.0f), 0.05f);
            glutPostRedisplay();
            break;
        case 'k':  //mover hacia izquierda
            escena->moverLuz(luzSeleccionada, glm::vec3(-0.1f, 0.0f, 0.0f), -0.05f);
            glutPostRedisplay();
            break;
        case 'u':  //mover hacia arriba
            escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, -0.1f, 0.0f), 0.05f);
            glutPostRedisplay();
            break;
        case 'j':  //mover hacia abajo
            escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.1f, 0.0f), -0.05f);
            glutPostRedisplay();
            break;
        case 'y':
            luzSeleccionada = (luzSeleccionada + 1) % escena->getNumLuces();
            std::cout << "\nSe ha seleccionado la luz: " << luzSeleccionada;
            break;
        case 'h':
            luzSeleccionada = (luzSeleccionada + 1) % escena->getNumLuces();
            std::cout << "\nSe ha seleccionado la luz: " << luzSeleccionada;
            break;

            //cambiar color
        case '7':  //hacer m�s rojo
            escena->incrementarColorDeTodasLasLuces(glm::vec3(0.05f, 0.0f, 0.0f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(0.05f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case '4':  ////hacer menos rojo
            escena->incrementarColorDeTodasLasLuces(glm::vec3(-0.05f, 0.0f, 0.0f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(-0.05f, 0.0f, 0.0f));
            glutPostRedisplay();
            break;
        case '8':  //hacer m�s verde
            escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.05f, 0.0f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.05f, 0.0f));
            glutPostRedisplay();
            break;
        case '5':  ////hacer menos verde
            escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, -0.05f, 0.0f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, -0.05f, 0.0f));
            glutPostRedisplay();
            break;
        case '9':  //hacer m�s azul
            escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.0f, 0.05f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.0f, -0.05f));
            glutPostRedisplay();
            break;
        case '6':  ////hacer menos azul
            escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.0f, -0.05f));
            //Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.0f, -0.05f));
            glutPostRedisplay();
            break;//*/
    }
}

void mouseFunc(int button, int state, int x, int y) {
    if (state == 0) {
        if (button == 0) {
            std::cout << "Se ha pulsado el bot�n ";
            //ponemos las variables en su valor correspondiente al hacer click, de modo que comenzaremos a contar los movimientos del rat�n desde el lugar correcto
            xZero = x;
            yZero = y;
        }
    } else {
        std::cout << "Se ha soltado el bot�n ";
    }

    if (button == 0) std::cout << "de la izquierda del rat�n " << std::endl;
    if (button == 1) std::cout << "central del rat�n " << std::endl;
    if (button == 2) std::cout << "de la derecha del rat�n " << std::endl;

    std::cout << "en la posici�n " << x << " " << y << std::endl
              << std::endl;
}

void mouseMotionFunc(int x, int y) {
    //calculamos el �ngulo de rotaci�n horizontal
    camera->viewRotation(0.005f * float(x - xZero), glm::vec3(0.0f, 1.0f, 0.0f));

    //calculamos el �ngulo de rotaci�n vertical
    float yShift = float(y - yZero) * 0.005f;  //variable que determina el cambio a aplicar
    yAngle += yShift;                          //�ngulo tras el cambio
    //se establece que no rote m�s de 90� y menos de -90� para evitar acabar mirando de espaldas, lo que invertir�a derecha e izquierda
    if (yAngle > glm::half_pi<float>()) {            //si nos pasamos de 90�,
        yShift -= (yAngle - glm::half_pi<float>());  //reducimos yShift en la cantidad correspondiente
        yAngle = glm::half_pi<float>();              //y dejamos la cuenta en 90�
    } else if (yAngle < -glm::half_pi<float>()) {    //si nos pasamos de -90�,
        yShift -= (yAngle + glm::half_pi<float>());  //reducimos yShift en la cantidad correspondiente
        yAngle = -glm::half_pi<float>();             //y dejamos la cuenta en -90�
    }

    camera->rotate(yShift, glm::vec3(1.0f, 0.0f, 0.0f));
    //se actualizan los valores de xZero e yZero para que se pueda calcular correctamente el distance en el pr�ximo movimiento de rat�n
    xZero = x;
    yZero = y;
}