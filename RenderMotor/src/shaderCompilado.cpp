#include "shaderCompilado.h"
#include "auxiliar.h"
#include <gl/glew.h>
#include <gl/gl.h>
#include <iostream>
#include <string>




ShaderCompilado::ShaderCompilado(const char *vname, const char *fname, int numeroDeLuces)
{
	initShader(vname, fname,numeroDeLuces);
}

ShaderCompilado::~ShaderCompilado()
{
	delete uLPos;
	delete uLDir;
	delete uLCol;
	delete uLAngle;

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);
}

int ShaderCompilado::loadShader(const char *fileName, int type) {
	//Lee el shader
	unsigned int fileLen;//longitud del shader
	char *source = loadStringFromFile(fileName, fileLen);//lee un archivo desde disco usando la funcion auxiliar, devuelve por parametro la longitud
														 //////////////////////////////////////////////
														 //Creación y compilación del Shader
	GLuint shader;//
	shader = glCreateShader(type);//crea un shader del tipo pasado por el argumento
	glShaderSource(shader/*identificador donde almacenar el shader*/, 1/*tenemos una unica cadena*/,
		(const GLchar **)&source/*codigo fuiente del shader*/, (const GLint *)&fileLen/*longitud del shader*/);
	glCompileShader(shader);//Decimos que compile el shader que hemos creado
	delete source;//borramos de memoria el shader porque ya esta compilado y no lo volvemos a usar

				  //Codigo para la comprobacion de errores
				  //Comprobamos que se compiló bien
	GLint compiled;//almacena el estado de la compilación
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);//almacena por referencia en la variable creada el estado de la compilación
	if (!compiled)//comprueba si ha habido error de comprobacion
	{
		//Calculamos una cadena de error
		GLint logLen;//longitud de la cadena de error
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);//almacena en la variable la longitud del error
		char *logString = new char[logLen];//reserva de memoria dinamica, reserva un array de la longitud del error
		glGetShaderInfoLog(shader, logLen, NULL, logString);//obtiene la cadena de error
		std::cout << "\n Error al compilar el shader: " << logString << std::endl;//muestra el error por consola
		delete logString;//elimina de memoria dinamica la cadena de error; no es necesario eliminar la longitud porque es memoria estatica
		glDeleteShader(shader);//borramos el shader ya que se ha compilado con error
		char a;
		std::cin >> a;
		exit(-1);//salimos con error
	}
	return shader;//devuelve el shader
}

void ShaderCompilado::initShader(const char *vname, const char *fname, int numeroDeLuces) {

	vshader = loadShader(vname, GL_VERTEX_SHADER);//crea el shader de vertices
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);//crea el shader de fragmentos

	program = glCreateProgram();//crea un programa de openGL
	glAttachShader(program, vshader);//linka el shader de vertices con el programa
	glAttachShader(program, fshader);//linka el shader de fragmentos con el programa

									 //Establecemos la posicion de los atributos variantes
									 //se debe hacer antes del linkado porque sino no conserva los nombres y posiciones
	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");

	glLinkProgram(program);//linkamos el programa

						   //Control de errores
	GLint linked;//indica si esta linkado
	glGetProgramiv(program, GL_LINK_STATUS, &linked);//almacena en linked si esta linkado
	if (!linked)//alamacena si ha habido un error de linkado
	{
		//Calculamos una cadena de error de forma similar a la de compilacion de shader
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		program = 0;//pone el programa a nulo
		exit(-1);//sale con codigo de error
	}

	//Obtenemos un localizador para cada una de las variable uniformes
	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	LucesActuales = numeroDeLuces;
	uLPos = new int[numeroDeLuces];
	uLDir = new int[numeroDeLuces];
	uLCol = new int[numeroDeLuces];
	uLAngle = new int[numeroDeLuces];

	for (int i = 0; i < numeroDeLuces;i++) {
		char a = char(int('1')+i);
		std::string aux = std::string("Posl").append(&a, &a + 1);
		uLPos[i] = glGetUniformLocation(program, &aux[0]);
		aux = std::string("Dirl").append(&a, &a + 1);
		uLDir[i] = glGetUniformLocation(program, &aux[0]);
		aux = std::string("Il").append(&a, &a + 1);
		uLCol[i] = glGetUniformLocation(program, &aux[0]);
		aux = std::string("Anglel").append(&a, &a + 1);
		uLAngle[i] = glGetUniformLocation(program, &aux[0]);
		//std::cout << "\n" << i << " - " << uLPos[i] << " - " << uLDir[i] << " - " << uLCol[i] << " - " << uLAngle[i];
	}
	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecTex = glGetUniformLocation(program, "specularTex");
	uNormTex = glGetUniformLocation(program, "normalTex");
	


	//Obtenemos un localizador para cada una de las variable variantes
	//Comprobamos de esta forma que estan definidos y conservan los valores que les hemos dado
	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");//Si no usamos coordenadas de textura devuelve -1
	inTangent = glGetAttribLocation(program, "inTangent");
}

unsigned int ShaderCompilado::getprogram()
{
	return program;
}

int ShaderCompilado::getuModelViewMat()
{
	return uModelViewMat;
}

int ShaderCompilado::getuModelViewProjMat()
{
	return uModelViewProjMat;
}

int ShaderCompilado::getuNormalMat()
{
	return uNormalMat;
}

int ShaderCompilado::getuLPos(int numero)
{
	if (numero<LucesActuales && numero>-1)
		return uLPos[numero];
	else
		return -1;
}

int ShaderCompilado::getuLCol(int numero)
{
	if (numero<LucesActuales && numero>-1)
		return uLCol[numero];
	else
		return -1;
}

int ShaderCompilado::getuLDir(int numero)
{
	if (numero<LucesActuales && numero>-1)
		return uLDir[numero];
	else
		return -1;
}

int ShaderCompilado::getuLAngle(int numero)
{
	if (numero<LucesActuales && numero>-1)
		return uLAngle[numero];
	else
		return -1;
}

int ShaderCompilado::getuColorTex()
{
	return uColorTex;
}

int ShaderCompilado::getuNormTex()
{
	return uNormTex;
}

int ShaderCompilado::getuEmiTex()
{
	return uEmiTex;
}

int ShaderCompilado::getuSpecTex()
{
	return uSpecTex;
}

int ShaderCompilado::getinPos()
{
	return inPos;
}

int ShaderCompilado::getinColor()
{
	return inColor;
}

int ShaderCompilado::getinNormal()
{
	return inNormal;
}

int ShaderCompilado::getinTexCoord()
{
	return inTexCoord;
}

int ShaderCompilado::getinTangent()
{
	return inTangent;
}

int ShaderCompilado::getProgram()
{
	return program;
}