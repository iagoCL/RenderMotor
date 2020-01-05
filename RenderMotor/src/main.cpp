#include "BOX.h"
#include "auxiliar.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "database.h"
#include "camara.h"
#include "nodo.h"
#include "malla.h"
#include "luz.h"
#include "shaderCompilado.h"
#include "textura.h"

#include "interpolacion.h"



//hacer una foto con  dos cubos uno detrás de otro con glEnable(GL_DEPTH_TEST); activado y otra desactivado para la memoria
//glFrontFace(GL_CCW);//caras hacia nosotros en Counter Clock Wise PONERLO EN LA MEMORIA

////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
////////////////////////////////////////////////////

///////////////////////////////////////////////////
// Funciones auxiliares
///////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void destroy();

Interpolacion* cubo1;
Interpolacion* cubo2;

Database* escena;

//Utiles para mover con el raton
int xZero=0;
int yZero=0;
//variable que mantiene el estado actual del eje y de la cámara (para evitar pasarse de +-90º)
float yAngle = 0;
int luzSeleccionada = 4;


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();

	escena = new Database();
	
	//numLuces = 4;
	Luz** luces = new Luz*[7];
	luces[0] = new Luz(glm::vec3(0.0f, 0.0f, 11.0f), glm::vec3(0.7f));//luz basica del shader 0
	luces[1] = new Luz(glm::vec3(-3.0, 3.0, 3.0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(3.0, -3.0, -3.0), 0.5f);//foco azul
	luces[2] = new Luz(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f));//luz puntual roja
	luces[3] = new Luz(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), 2);//luz direccional verde
	luces[4] = new Luz(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0, 0.0, -1.0), 0.08f);//foco normal rojo
	luces[5] = new Luz(glm::vec3(-7.0, 0.0, 3.0), glm::vec3(0.8f));//luz puntual normal 
	luces[6] = new Luz(glm::vec3(0.0, -1.0, -1.0), glm::vec3(0.5f), 2);//luz direccional normal
	for (int i = 0; i < 7; i++)
		escena->añadirLuz(luces[i]);
	delete luces;
	//numShaders = 3;
	ShaderCompilado** shadersCompilados = new ShaderCompilado*[4];
	shadersCompilados[0] = new ShaderCompilado("../shaders/shader.v0.vert", "../shaders/shader.v0.frag");
	shadersCompilados[1] = new ShaderCompilado("../shaders/shader.v1.vert", "../shaders/shader.v1.frag");
	shadersCompilados[2] = new ShaderCompilado("../shaders/shader.v2.vert", "../shaders/shader.v2.frag", 3);
	shadersCompilados[3] = new ShaderCompilado("../shaders/shader.v3.vert", "../shaders/shader.v3.frag", 3);
	for(int i=0;i<4;i++)
		escena->añadirShader(shadersCompilados[i]);
	delete shadersCompilados;
	int numMallas = 4;;
	Malla* malla = new Malla(cubeNTriangleIndex, cubeNVertex, &(cubeTriangleIndex[0]), &(cubeVertexColor[0]), &(cubeVertexNormal[0]), &(cubeVertexPos[0]), &(cubeVertexTangent[0]), &(cubeVertexTexCoord[0]));
	Malla** mallas = new Malla*[numMallas];
	mallas[0] = malla->clone();//shader bump mapping
	mallas[1] = malla->clone();//shader luces colores
	mallas[2] = malla->clone();//shader luces colores
	mallas[3] = malla->clone();//shader simple
	Malla** assimp = Malla::mallaAssimp(numMallas, "../img/character/preacherCharacterB.obj", true, true);
	std::cout << numMallas;
	for (int i = 4; i < numMallas; i++)
		mallas[i] = assimp[i - 4];
	delete assimp;
	Textura* texturaAux1 = new Textura("../img/color2.png");
	Textura** texturasColores = new Textura*[numMallas];
	texturasColores[0] = texturaAux1;//shader bump mapping
	texturasColores[1] = texturaAux1->clone();//shader luces colores
	texturasColores[2] = texturaAux1->clone();//shader luces colores
	texturasColores[3] = nullptr;//shader simple
	texturasColores[4] = new Textura("../img/character/maps/bodyDiffuseMap.jpg", true);
	texturasColores[5] = new Textura("../img/character/maps/faceDiffuseMap.jpg", true);
	texturasColores[7] = new Textura("../img/character/maps/handsDiffuseMap.jpg", true);
	texturasColores[6] = new Textura("../img/character/maps/feetDiffuseMap.jpg", true);
	texturasColores[8] = new Textura("../img/character/maps/legsDiffuseMap.jpg", true);

	Textura* texturaAux2 = new Textura("../img/emissive.png");
	Textura* texturaAux3 = new Textura("../img/black.png", false);//imagen en negro
	Textura** texturasEmis = new Textura*[numMallas];
	texturasEmis[0] = texturaAux2;//shader bump mapping
	texturasEmis[1] = texturaAux2->clone();//shader luces colores
	texturasEmis[2] = texturaAux2->clone();//shader luces colores
	texturasEmis[3] = nullptr;//shader simple
	texturasEmis[4] = texturaAux3->clone();
	texturasEmis[5] = texturaAux3->clone();
	texturasEmis[6] = texturaAux3->clone();
	texturasEmis[7] = texturaAux3->clone();
	texturasEmis[8] = texturaAux3->clone();
	
	Textura* texturaAux4 = new Textura("../img/specMap.png");
	Textura**texturasSpec = new Textura*[numMallas];
	texturasSpec[0] = texturaAux4;//shader bump mapping
	texturasSpec[1] = texturaAux4->clone();//shader luces colores
	texturasSpec[2] = texturaAux4->clone();//shader luces colores
	texturasSpec[3] = nullptr;//shader simple
	texturasSpec[4] = texturaAux3->clone();
	texturasSpec[5] = texturaAux3->clone();
	texturasSpec[6] = texturaAux3->clone();
	texturasSpec[7] = texturaAux3->clone();
	texturasSpec[8] = texturaAux3->clone();

	Textura** texturasNorm = new Textura*[numMallas];
	texturasNorm[0] = new Textura("../img/normal.png");//shader bump mapping
	texturasNorm[1] = nullptr;//shader luces colores
	texturasNorm[2] = nullptr;//shader luces colores
	texturasNorm[3] = nullptr;//shader simple
	texturasNorm[4] = nullptr;
	texturasNorm[5] = nullptr;
	texturasNorm[6] = nullptr;
	texturasNorm[7] = nullptr;
	texturasNorm[8] = nullptr;

	for (int i = 0; i < numMallas; i++)
		escena->añadirMallaYTextura(mallas[i], texturasColores[i], texturasEmis[i], texturasSpec[i], texturasNorm[i]);

	escena->añadirNodo(1, 0, 3, 4, 3);//cubo en el centro orbitando en bump mapping
	escena->añadirNodo(2, 1, 3, 1, 2);//cubos en orbitra iluminados por luces de distinto tipo con colores
	escena->añadirNodo(1, 3, 1, 0, 0);//cubo basico en orbita
	escena->añadirNodo(numMallas-4, 4, 3, 4, 2);//modelo de goku cargado usando assimp
	
	//Escena->cambiarTexturaColor(6, TexturasNorm[0]);
	//Escena->eliminarNodo(0);
	
	escena->setProjDeCamara(60.0f, 1.0f, 0.1f, 100.0f);

	cubo1 = new Interpolacion(7.0f);
	cubo2 = new Interpolacion(4.0f);

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
	glutInitContextVersion(3, 3);//usamos opengl 3.3
								 //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); Para usar 3.1 con opciones de 3.2
	glutInitContextProfile(GLUT_CORE_PROFILE);//Solo usa core; no tiene retro compatibilidad

											  //cramos el frame buffer y la ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//indicamos que uaremos doble buffer, rgba, y almacenaremos la profundidad
	glutInitWindowSize(500, 500);//creamos la ventana; el contexto no es eficaz hasta crear la ventana
	glutInitWindowPosition(0, 0);//arriba a la izquierda
	glutCreateWindow("Prácticas GLSL");

	//inicializamos Glew y sus extensiones
	glewExperimental = GL_TRUE;//activamos la extensiones experimentales; usa las extensiones aunque estas sean experimentales
	GLenum err = glewInit();//inicializamos glew y vemos si hay error
	if (GLEW_OK != err)//comprueba si hay error
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;//muestra el mensaje de error si hay error
		exit(-1);//sale con un codigo de error
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);//obtiene la version de opengl que estaremos usando si no ha habido ningun error
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;//muestra la version por pantalla

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

	glFrontFace(GL_CCW);//pinta caras hacia nosotros en Counter Clock Wise 
	//glFrontFace(GL_CW);//pinta caras hacia nosotros en Clock Wise; pinta la parte de atras
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//indica que se rendericen ambas caras y que los polygonos se rellenen; poner solo front esta deprecated; hay que poner front y back
	glEnable(GL_CULL_FACE);//activamos el culling para las caras de atras

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

	escena->renderizarTodos();

	//cambia entre el buffer front y el back
	//Es una llamada bloqueante, espera a que acabe todo para llamarse
	glutSwapBuffers();
}

void resizeFunc(int width, int height) {
	glViewport(0, 0, width, height);

	escena->setProjDeCamara(60.0, float(width) / float(height), 0.1f, 100.0f);

	glutPostRedisplay();
}

void idleFunc() {
	glm::mat4 model1 = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle >3.141592f *2.0f) ? 0 : angle + 0.1f;
	model1 = glm::rotate(model1, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	//model 2
	static float angle2 = 0.0f; //rotacion
	angle2 = (angle2>3.141592f *2.0f) ? 0 : angle2 + 0.03f;
	glm::mat4 model2= glm::rotate(glm::translate(glm::mat4(1.0f),/*vector de radio*/ glm::vec3(5.0f, 0.0f, 0.0f)), angle2, glm::vec3(0.0f, 1.0f, 0.0f));//model2=Trasladar el radio*rotar, 

	static float translangle = 0.0f; //angulo de translación
	translangle = (translangle>3.141592f *2.0f) ? 0 : translangle + 0.005f;

	model2 = glm::rotate(glm::mat4(1.0f), translangle, glm::vec3(0.0f, 1.0f, 0.0f))*model2; //rota la matriz para girar el cubo
	
	cubo2->incrementar(0.05f);
	cubo1->incrementar(0.05f);

	glm::mat4 model3 =glm::translate(glm::mat4(1.0f), cubo1->splinesCubicos() )*model1;//model2=Trasladar el radio*rotar, 
	glm::mat4 model4 = glm::translate(glm::mat4(1.0f), cubo2->bezier() )*model1;//model2=Trasladar el radio*rotar, 

	
	escena->cambiarModel(0, model1);
	escena->cambiarModel(1,model2);
	escena->cambiarModel(2, model3);
	escena->cambiarModel(3, model4);
	glutPostRedisplay();

}

void keyboardFunc(unsigned char key, int x, int y) {
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
	//los glm::rotate y glm::translate se podrían cambiar por la matriz hardcodeada, pero así queda más claro
	switch (key) {
	case 'w': //desplazar hacia delante
		escena->desplazarCamara(glm::vec3(0.0f, 0.0f, 1.0f));
		glutPostRedisplay();
		break;
	case 's': //desplazar hacia atras
		escena->desplazarCamara(glm::vec3(0.0f, 0.0f, -1.0f));
		glutPostRedisplay();
		break;
	case 'a': //desplazar hacia la izquierda
		escena->desplazarCamara(glm::vec3(1.0f, 0.0f, 0.0f));
		glutPostRedisplay();
		break;
	case 'd': //desplazar hacia la derecha
		escena->desplazarCamara(glm::vec3(-1.0f, 0.0f, 0.0f));
		glutPostRedisplay();
		break;
	case 'e': //rotar hacia la derecha
		escena->rotarCamaraUsandoView(-0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		glutPostRedisplay();
		break;
	case 'q': //rotar hacia la izquierda
		escena->rotarCamaraUsandoView(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
		glutPostRedisplay();
		break;

		//mover luz	
	case 'o'://mover hacia delante
		escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.0f, -1.0f), 0.05f);
		glutPostRedisplay();
		break;
	case 'l'://mover hacia detras
		escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.0f, 1.0f), - 0.05f);
		glutPostRedisplay();
		break;
	case 'i'://mover hacia la  derecha
		escena->moverLuz(luzSeleccionada, glm::vec3(0.1f, 0.0f, 0.0f), 0.05f);
		glutPostRedisplay();
		break;
	case 'k'://mover hacia izquierda
		escena->moverLuz(luzSeleccionada, glm::vec3(-0.1f, 0.0f, 0.0f), - 0.05f);
		glutPostRedisplay();
		break;
	case 'u'://mover hacia arriba
		escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, -0.1f, 0.0f), 0.05f);
		glutPostRedisplay();
		break;
	case 'j'://mover hacia abajo
		escena->moverLuz(luzSeleccionada, glm::vec3(0.0f, 0.1f, 0.0f), - 0.05f);
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
	case '7'://hacer más rojo
		escena->incrementarColorDeTodasLasLuces(glm::vec3(0.05f, 0.0f, 0.0f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(0.05f, 0.0f, 0.0f));
		glutPostRedisplay();
		break;
	case '4':////hacer menos rojo
		escena->incrementarColorDeTodasLasLuces(glm::vec3(-0.05f, 0.0f, 0.0f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(-0.05f, 0.0f, 0.0f));
		glutPostRedisplay();
		break;
	case '8'://hacer más verde
		escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.05f, 0.0f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.05f, 0.0f));
		glutPostRedisplay();
		break;
	case '5':////hacer menos verde
		escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, -0.05f, 0.0f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, -0.05f, 0.0f));
		glutPostRedisplay();
		break;
	case '9'://hacer más azul
		escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.0f, 0.05f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.0f, -0.05f));
		glutPostRedisplay();
		break;
	case '6':////hacer menos azul
		escena->incrementarColorDeTodasLasLuces(glm::vec3(0.0f, 0.0f, -0.05f));
		//Escena->incrementarColorDeLuz(4, glm::vec3(0.0f, 0.0f, -0.05f));
		glutPostRedisplay();
		break;

	}

}

void mouseFunc(int button, int state, int x, int y) {
	if (state == 0) {
		if (button == 0) {
			std::cout << "Se ha pulsado el botón ";
			//ponemos las variables en su valor correspondiente al hacer click, de modo que comenzaremos a contar los movimientos del ratón desde el lugar correcto
			xZero = x;
			yZero = y;
		}
	}
	else {
		std::cout << "Se ha soltado el botón ";
	}

	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y) {


	//calculamos el ángulo de rotación horizontal
	escena->rotarCamaraUsandoView(0.005f*float(x - xZero), glm::vec3(0.0f, 1.0f, 0.0f));

	//calculamos el ángulo de rotación vertical
	float yShift = float(y - yZero)*0.005f; //variable que determina el cambio a aplicar
	yAngle += yShift; //ángulo tras el cambio
	//se establece que no rote más de 90º y menos de -90º para evitar acabar mirando de espaldas, lo que invertiría derecha e izquierda
	if (yAngle > 3.1416 / 2.0f) { //si nos pasamos de 90º,
		yShift -= (yAngle - 3.1416 / 2); //reducimos yShift en la cantidad correspondiente
		yAngle = 3.1416 / 2; //y dejamos la cuenta en 90º
	}
	else if (yAngle < -3.1416 / 2.0f) { //si nos pasamos de -90º,
		yShift -= (yAngle + 3.1416 / 2); //reducimos yShift en la cantidad correspondiente
		yAngle = -3.1416 / 2; //y dejamos la cuenta en -90º
	}

	escena->rotarCamara(yShift, glm::vec3(1.0f, 0.0f, 0.0f));
	//se actualizan los valores de xZero e yZero para que se pueda calcular correctamente el desplazamiento en el próximo movimiento de ratón
	xZero = x;
	yZero = y;

}