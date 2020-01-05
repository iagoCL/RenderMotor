#include "Malla.h"

#include <gl/glew.h>
#include <gl/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/mesh.h>
#include <Assimp/color4.h>

Malla::Malla(const int numCarasA, const int numVerticesA, const unsigned int *indiceTriangulosA = nullptr, const float *indiceColoresA = nullptr, const float *indiceNormalesA = nullptr, const float *indicePosicionesA = nullptr, const float *indiceTangentesA = nullptr, const float *indiceTexCoordA = nullptr)
{
	numCaras = numCarasA;
	numVertices = numVerticesA;
	indiceTriangulos = indiceTriangulosA;
	indiceColores = indiceColoresA;
	indiceNormales = indiceNormalesA;
	indicePosiciones = indicePosicionesA;
	indiceTangentes = indiceTangentesA;
	indiceTexCoord = indiceTexCoordA;
}

Malla::Malla()
{
}

Malla::Malla(aiMesh* c)
{
	const int MAXIMO_DE_TRIANGULOS = 1000;
	const int MAXIMO_DE_VERTICES = 3000;
	const int MAXIMO_DE_VERTICES_POR_2 = 6000;
	const int MAXIMO_DE_VERTICES_POR_3 = 9000;

	//aiMesh *c = scene->mMeshes[0];//almacena la malla del objeto
	std::cout << "Se ha obtenido la malla del modelo:" << std::endl;

	//NumCaras numero de triangulos del modelo, se corresponde con el numero de caras 
	numCaras = c->mNumFaces; //324 en Goku
	std::cout << "El modelo tiene " << numCaras << " caras; calculando el numero de vértices..." << std::endl;

	//NumVertices numero total de vertices
	numVertices = c->mNumVertices;
	std::cout << "El modelo tiene " << numVertices << " vértices; calculando el array de triángulos..." << std::endl;

	//IndiceTriangulos es un vector de enteros que indica el numero de los vertices en el orden de las caras
	unsigned int* IndiceTriangulosAux = new unsigned int[numCaras*3];//MAXIMO DE VERTICES=MAXIMO DE CARAS o triangulo * 3 vertices cada una
	for (int i = 0; i < numCaras; i++) {
		IndiceTriangulosAux[3 * i] = c->mFaces[i].mIndices[0];
		IndiceTriangulosAux[3 * i + 1] = c->mFaces[i].mIndices[1];
		IndiceTriangulosAux[3 * i + 2] = c->mFaces[i].mIndices[2];
	}

	indiceTriangulos = IndiceTriangulosAux;
	std::cout << "Se ha calculado el array de triángulos; calculando el array de posiciones de los vértices..." << std::endl;

	//IndicePosiciones es un array de flotantes en el que cada tres elementos dan el vector de cada punto
	float* ArrayDePosicionesAux=new float[numVertices*3];//MAXIMO_DE_VERTICES * 3 coordenadas cada uno
	for (int i = 0; i < numVertices; i++) {
		ArrayDePosicionesAux[3 * i] = c->mVertices[i].x;
		ArrayDePosicionesAux[3 * i + 1] = c->mVertices[i].y;
		ArrayDePosicionesAux[3 * i + 2] = c->mVertices[i].z;
	}

	indicePosiciones = ArrayDePosicionesAux;
	std::cout << "Se ha calculado el array de posiciones de los vértices; calculando el array de colores de cada vértice..." << std::endl;

	//IndiceColores array de floats que indica el color de los distintos vertices en formato rgb 3 elementos para el color de cada vertice
	float* ArrayDeColorDeVertices=new float[numVertices*3];//MAXIMO_DE_VERTICES * 3  componentes rgb 
	for (int i = 0; i < numVertices; i++) {
		/*El modelo como es comun no le asigna color a los vertices sino que usa las coordenadas de textura por lo que el siguiente codigo da error
		ArrayDeColorDeVertices[3 * i    ] = c->mColors[0][i].r;
		ArrayDeColorDeVertices[3 * i + 1] = c->mColors[0][i].g;
		ArrayDeColorDeVertices[3 * i + 2] = c->mColors[0][i].b;
		Se asigna a todos los vertices un color gris*/
		ArrayDeColorDeVertices[3 * i] = 0.5f;
		ArrayDeColorDeVertices[3 * i + 1] = 0.5f;
		ArrayDeColorDeVertices[3 * i + 2] = 0.5f;
	}

	indiceColores = ArrayDeColorDeVertices;
	std::cout << "Se ha calculado el array de colores de los vértices; calculando el array de normales de cada vertice..." << std::endl;

	// cubeVertexNormal es un vector de flotantes que le asigna a cada vertice una normal obtenida de juntar 3 de sus elementos
	float* ArrayDeNormalesDeVerticesAux = new float[numVertices*3];//MAXIMO_DE_VERTICES * 3  componentes xyz
	for (int i = 0; i < numVertices; i++) {
		ArrayDeNormalesDeVerticesAux[3 * i] = c->mNormals[i].x;
		ArrayDeNormalesDeVerticesAux[3 * i + 1] = c->mNormals[i].y;
		ArrayDeNormalesDeVerticesAux[3 * i + 2] = c->mNormals[i].z;
	}
	indiceNormales = ArrayDeNormalesDeVerticesAux;
	std::cout << "Se ha calculado el array de normales de los vértices, calculando el array de coordenadas de textura de cada vertice..." << std::endl;

	//cubeVertexTexCoord es un vector de flotantes que le asigna a cada vertice unas coordenadas de textura obtenida de juntar 2 de sus elementos
	float* ArrayDeCoordTexturaAux = new float[2*numVertices];//MAXIMO_DE_VERTICES * 2  componentes de coordenadas de textura
	for (int i = 0; i < numVertices; i++) {
		ArrayDeCoordTexturaAux[2 * i] = c->mTextureCoords[0][i].x;
		ArrayDeCoordTexturaAux[2 * i + 1] = c->mTextureCoords[0][i].y;
	}

	indiceTexCoord = ArrayDeCoordTexturaAux;
	std::cout << "Se ha calculado el array de coordenadas de texturas de los vértices; calculando el array de tangentes de cada vértice..." << std::endl;

	//cubeVertexTangent es un vector de flotantes que le asigna a cada vertice una tangente obtenida de juntar 3 de sus elementos
	float* ArrayDeTangentesDeVerticesAux = new float[numVertices*3];//MAXIMO_DE_VERTICES * 3  componentes xyz
	for (int i = 0; i < numVertices; i++) {
		/*ArrayDeTangentesDeVerticesAux[3 * i] = c->mTangents[i].x;
		ArrayDeTangentesDeVerticesAux[3 * i + 1] = c->mTangents[i].y;
		ArrayDeTangentesDeVerticesAux[3 * i + 2] = c->mTangents[i].z;*/

		//como no contiene tangentes voy a emplear la perpendiloar de las normales
		ArrayDeTangentesDeVerticesAux[3 * i] = - c->mNormals[i].z;
		ArrayDeTangentesDeVerticesAux[3 * i + 1] = 0.0f;
		ArrayDeTangentesDeVerticesAux[3 * i + 2] = c->mNormals[i].x;
		}
	indiceTangentes = ArrayDeTangentesDeVerticesAux;
	std::cout << "Se ha calculado el array de tangentes de los vértices; se procedera a crear el objeto con los valores calculados." << std::endl;
}

Malla** Malla::mallaAssimp(int &numeroDeMallas,std::string rutaAObj,bool recalcular, bool simplificacion)
{
	std::cout << "\nHa comenzado el proceso de carga del modelo; puede llevar unos segundos... " << std::endl;//voy lanzando mensajes de consola para controlar la importacion que tarda mucho
	Assimp::Importer importer;//creo el importador
	const aiScene* scene = importer.ReadFile(rutaAObj, 0);//0 indica no post-proceso

	if (!scene) {
		std::cout << (importer.GetErrorString());//si hay un error al cargar el archivo lo muestra por consola
		return nullptr;
	}

	else {
		std::cout << "El modelo se ha cargado desde disco; tratando de hallar las mallas..." << std::endl;
		int numMallasAux = scene->mNumMeshes;
		numeroDeMallas += scene->mNumMeshes;
		std::cout << "El modelo se ha cargado desde disco; tiene " << numMallasAux << " mallas." << std::endl;
		Malla** devolucion = new Malla*[numMallasAux];
		for (int i = 0; i < numMallasAux; i++) {
			std::cout << std::endl << std::endl << std::endl << "Se procedera a cargar la malla " << i + 1 << std::endl << std::endl;
			devolucion[i]=new Malla(scene->mMeshes[i]);
			if (recalcular) {
				devolucion[i]->RecalcularNormalesYTangentes(simplificacion);
			}
		}
		std::cout << std::endl << std::endl << std::endl << "Se han cargado todas las mallas." << std::endl << std::endl;

		return devolucion;
	}
}


Malla::~Malla()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &posVBO);//solo destruye si hemos llegado a crearlos
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &normalVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	std::cout << "\nBorrado de malla: eliminacion de OpenGL hecha";
}

void Malla::initObj(int inPos = -1, int inColor = -1, int inNormal = -1, int inTexCoord = -1, int inTangent = -1) {
	glGenVertexArrays(1/*crea solo 1*/, &vao);//genera un array de vertices
	glBindVertexArray(vao);

	if (inPos != -1)//comprueba que se ha inicializado y linkado correctamente
	{
		glGenBuffers(1/*tamaño del vertice*/, &posVBO);//generamos un buffer
		glBindBuffer(GL_ARRAY_BUFFER/*usamos el buffer activo*/, posVBO);//lo activamos
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3/*tamaño de lo que vamos a subir*/,
			indicePosiciones, GL_STATIC_DRAW);//subida de datos que no se emplea en el buffer
		glVertexAttribPointer(inPos, 3/*tres coordenadas*/, GL_FLOAT, GL_FALSE, 0, 0);//almacena el buffer
		glEnableVertexAttribArray(inPos);//habilitamos el uso del atributo ya que existe
	}

	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3,
			indiceColores, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}

	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3,
			indiceNormales, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}

	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 2,
			indiceTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	if (inTangent != -1)//comprueba que se ha inicializado y linkado correctamente
	{
		glGenBuffers(1/*tamaño del vertice*/, &tangentVBO);//generamos un buffer
		glBindBuffer(GL_ARRAY_BUFFER/*usamos el buffer activo*/, tangentVBO);//lo activamos
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3/*tamaño de lo que vamos a subir*/,
			indiceTangentes, GL_STATIC_DRAW);//subida de datos que no se emplea en el buffer
		glVertexAttribPointer(inTangent, 3/*tres coordenadas*/, GL_FLOAT, GL_FALSE, 0, 0);//almacena el buffer
		glEnableVertexAttribArray(inTangent);//habilitamos el uso del atributo ya que existe
	}

	//crea la lista de vertices
	glGenBuffers(1, &triangleIndexVBO);//creamos el buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);//lo activamos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numCaras * sizeof(unsigned int) * 3, indiceTriangulos, GL_STATIC_DRAW);//subimos los datos
}

void Malla::setModel(glm::mat4 nuevaModel){
	model = nuevaModel;
}

void Malla::renderizarMalla(glm::mat4 view, glm::mat4 proj, unsigned int IdModelView = -1, unsigned int IdModelViewProj = -1, unsigned int IdNormal = -1) {
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	if (IdModelView != -1)
		glUniformMatrix4fv(IdModelView, 1, GL_FALSE, &(modelView[0][0]));
	if (IdModelViewProj != -1)
		glUniformMatrix4fv(IdModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
	if (IdNormal != -1)
		glUniformMatrix4fv(IdNormal, 1, GL_FALSE, &(normal[0][0]));

}

Malla* Malla::clone()
{
	Malla* copia=new Malla ();
	copia->vao=vao;
	//VBOs que forman parte del objeto
	copia->posVBO = posVBO;
	copia->colorVBO = colorVBO;
	copia->normalVBO = normalVBO;
	copia->texCoordVBO = texCoordVBO;
	copia->triangleIndexVBO = triangleIndexVBO;
	copia->tangentVBO = tangentVBO;

	copia->numCaras = numCaras;
	copia->numVertices = numVertices;
	copia->indiceTriangulos = indiceTriangulos;
	copia->indiceColores = indiceColores;
	copia->indiceNormales = indiceNormales;
	copia->indicePosiciones = indicePosiciones;
	copia->indiceTangentes = indiceTangentes;
	copia->indiceTexCoord = indiceTexCoord;
	return copia;
}

int Malla::getNumCaras()
{
	return numCaras;
}

int Malla::getVAO()
{
	return vao;
}

void Malla::RecalcularNormalesYTangentes(bool simplificar) {
	glm::vec3* tangenteAlVertice = new glm::vec3[numVertices];
	glm::vec3* normalAlVertice = new glm::vec3[numVertices];
	glm::vec3* CoordenadasDelVertice = new glm::vec3[numVertices];
	std::cout << "Recalculando tangentes y vértices...";
	//inicializo ambos al vector nulo
	for (int i = 0; i < numVertices; i++) {
		tangenteAlVertice[i] = glm::vec3(0.0f);
		normalAlVertice[i] = glm::vec3(0.0f);
		CoordenadasDelVertice[i] = glm::vec3(indicePosiciones[3 * i], indicePosiciones[3 * i + 1], indicePosiciones[3 * i + 2]);
	}

	//recorro todos los triángulos y voy calculando su normal y tangente, y asignándolselas a cada punto
	for (int i = 0; i < numCaras; i++) {
		//Calculo los vectores de los lados del triángulo
		//IndiceTriangulos[i] devuelve el int que identifica al triangulo i
		//CoordenadasDelVertice[a] devuelve el glm::vec3 de la posicion del punto a
		glm::vec3 tangenteA = CoordenadasDelVertice[indiceTriangulos[3 * i + 1]] - CoordenadasDelVertice[indiceTriangulos[3 * i]];//tangente del punto i al punto i+1
		glm::vec3 tangenteB = CoordenadasDelVertice[indiceTriangulos[3 * i + 2]] - CoordenadasDelVertice[indiceTriangulos[3 * i]];//tangente del punto i al punto i+2
		glm::vec3 tangenteC = CoordenadasDelVertice[indiceTriangulos[3 * i + 2]] - CoordenadasDelVertice[indiceTriangulos[3 * i + 1]];//tangente del punto i+1 al punto i+2

		//En teoría estos vectores son tangentes al triángulo, por lo que podrían asignarse
		//Como hay infinitos vectores tangentes, se suele escoger el que su direccion coincide con las de las coordenadas de textura
		//IndiceTexCoord[2*i] devuelve la coordenada de textura X del punto i;  IndiceTexCoord[2*i+1] devuelve la coordenada de textura Y del punto i
		float t1 = indiceTexCoord[2 * indiceTriangulos[3 * i + 1]] - indiceTexCoord[2 * indiceTriangulos[3 * i]];//textcoord[i+1].x - textcoord[i].x;
		float t2 = indiceTexCoord[2 * indiceTriangulos[3 * i + 2]] - indiceTexCoord[2 * indiceTriangulos[3 * i]];// textcoord[i+2].x - textcoord[i].x;

		glm::vec3 vecAux = t2*tangenteA - t1*tangenteB;//formula consultada en mathematics for 3d Game Programing and computer graphics en el cap 7.8

		//ponderacion otorgada en el libro; emplearemos otro método para no calcular bitangentes
		//float t3 = IndiceTexCoord[2*IndiceTriangulos[3*i + 1] + 1] - IndiceTexCoord[2*IndiceTriangulos[3 * i] + 1];//textcoord[i+1].y - textcoord[i].y;
		//float t4 = IndiceTexCoord[2*IndiceTriangulos[3 * i + 2] + 1] - IndiceTexCoord[2*IndiceTriangulos[3 * i] + 1];// textcoord[i+2].y - textcoord[i].y;
		//float t5 = 1.0f / (t1 * t4 - t2 * t3);
		//vecAux *= t5;

		vecAux = glm::normalize(vecAux); //si quisieramos que todos los triangulos tuvieran la misma ponderacion, no añadiríamos nada
		//dado que queremos que las tangentes se ponderen en funcion del angulo de cada vertice del triangulo,
		//sumo la tangente multiplicada por el ángulo a sus correspondientes valores
		tangenteAlVertice[indiceTriangulos[3 * i]] += glm::angle(glm::normalize(tangenteA), glm::normalize(tangenteB))*vecAux;//Si quisieramos que las tangentes se ponderasen en funcion del angulo del angulo de cada vertice del triangulo
		tangenteAlVertice[indiceTriangulos[3 * i + 1]] += glm::angle(glm::normalize(tangenteC), glm::normalize(-tangenteA))*vecAux;
		tangenteAlVertice[indiceTriangulos[3 * i + 2]] += glm::angle(glm::normalize(tangenteC), glm::normalize(tangenteB))*vecAux;

		//reciclamos vecAux para su uso con normales
		//vecAux = glm::cross(tangenteA, tangenteB);//Si lo dejáramos así ponderaríamos en fucion del area de los triangulos
		vecAux = glm::normalize(glm::cross(tangenteA, tangenteB));//Si lo dejáramos así todas ponderarían lo mismo

		//dado que queremos que las normales se ponderen en funcion del angulo de cada vertice del triangulo,
		//sumo la normal multiplicada por el ángulo a sus correspondientes valores	
		normalAlVertice[indiceTriangulos[3 * i]] += glm::angle(glm::normalize(tangenteA), glm::normalize(tangenteB))*vecAux;//Si quisieramos que las tangentes se ponderasen en funcion del angulo del angulo de cada vertice del triangulo
		normalAlVertice[indiceTriangulos[3 * i + 1]] += glm::angle(glm::normalize(tangenteC), glm::normalize(-tangenteA))*vecAux;
		normalAlVertice[indiceTriangulos[3 * i + 2]] += glm::angle(glm::normalize(tangenteC), glm::normalize(tangenteB))*vecAux;

	}

	if (simplificar) {
		//mejoramos el cálculo de normales para solventar problemas relativos a duplicidad de vértices
		//variable auxiliar (cuenta de vertices almacenados en array auxiliar)
		int c = 0;
		//arrays auxiliares
		glm::vec3* tempVecStorage = new glm::vec3[numVertices];
		glm::vec3* tempNormalStorage = new glm::vec3[numVertices];
		glm::vec3* tempTangentStorage = new glm::vec3[numVertices];

		//recorremos todos los vértices y sumamos las normales de aquellos con posiciones idénticas
		//tras calcularlo, subimos la información a un array auxiliar; si ya existiese información para un vertice con unas coord
		//determinadas, aplicamos lo calculado anteriormente para el mismo.
		for (int i = 0; i < numVertices; i++) {
			bool t = false;
			for (int m = 0; m < c && !t; m++) {
				if (CoordenadasDelVertice[i] == tempVecStorage[m]) {
					normalAlVertice[i] = tempNormalStorage[m];
					tangenteAlVertice[i] = tempTangentStorage[m];
					t = true;
				}
			}
			if (!t) {
				for (int n = i + 1; n < numVertices; n++) {
					if (CoordenadasDelVertice[n] == CoordenadasDelVertice[i]) {
						normalAlVertice[i] += normalAlVertice[n];
						tangenteAlVertice[i] += tangenteAlVertice[n];
					}
				}
				tempVecStorage[c] = CoordenadasDelVertice[i];
				tempNormalStorage[c] = normalAlVertice[i];
				tempTangentStorage[c] = tangenteAlVertice[i];
				c++;
			}
		}
		delete tempVecStorage;
		delete tempNormalStorage;
		delete tempTangentStorage;
	}



	float* IndiceNormales1 = new float[numVertices * 3];
	float* IndiceTangentes1 = new float[numVertices * 3];

	//recorro todos los arrays de normales y tangentes, los normalizo y transformo en un array de floats
	for (int i = 0; i < numVertices; i++) {

		normalAlVertice[i] = glm::normalize(normalAlVertice[i]);//se calcula como una suma ponderada y luego se cambia su modulo a q para hallar la media ponderada al normalizar

		//basándonos en el libro, aplicamos una ortogonalización de Gram-Schmidt
		tangenteAlVertice[i] = glm::normalize(tangenteAlVertice[i] - normalAlVertice[i] * glm::dot(normalAlVertice[i], tangenteAlVertice[i]));

		//como la normal y la tangente se calculan en paralelo es posible que estas no sean perpendiculares
		//comprobamos que normal y tangente sean perpendiculares; si no lo fueren, muestro un mensaje de error por consola
		if (glm::angle(normalAlVertice[i], tangenteAlVertice[i]) > 1.07*3.1416 / 2 || glm::angle(normalAlVertice[i], tangenteAlVertice[i]) < 0.93*3.1416 / 2) {
			std::cout << "ERROR en el vértice " << i << ": el ángulo entre la tangete y la normal es " << glm::angle(normalAlVertice[i], tangenteAlVertice[i]) << std::endl;
		}

		IndiceNormales1[3 * i] = normalAlVertice[i].x;
		IndiceNormales1[3 * i + 1] = normalAlVertice[i].y;
		IndiceNormales1[3 * i + 2] = normalAlVertice[i].z;

		IndiceTangentes1[3 * i] = tangenteAlVertice[i].x;
		IndiceTangentes1[3 * i + 1] = tangenteAlVertice[i].y;
		IndiceTangentes1[3 * i + 2] = tangenteAlVertice[i].z;
	}
	delete indiceNormales;
	indiceNormales = IndiceNormales1;
	delete indiceTangentes;
	indiceTangentes = IndiceTangentes1;
	
}


