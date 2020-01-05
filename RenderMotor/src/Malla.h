#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/mesh.h>
#include <Assimp/color4.h>

class Malla
{
private:
	unsigned int vao;
	//VBOs que forman parte del objeto
	unsigned int posVBO;
	unsigned int colorVBO;
	unsigned int normalVBO;
	unsigned int texCoordVBO;
	unsigned int triangleIndexVBO;
	unsigned int tangentVBO;

	int numCaras;
	int numVertices;
	const unsigned int *indiceTriangulos = nullptr;
	const float *indiceColores = nullptr;
	const float *indiceNormales = nullptr;
	const float *indicePosiciones = nullptr;
	const float *indiceTangentes = nullptr;
	const float *indiceTexCoord = nullptr;

	glm::mat4 model = glm::mat4(1.0f);
public:
	Malla(const int NumCarasA, const int NumVerticesA, const unsigned int * IndiceTriangulosA, const float * IndiceColoresA, const float * IndiceNormalesA, const float * IndicePosicionesA, const float * IndiceTangentesA, const float * IndiceTexCoordA);
	Malla();
	Malla(aiMesh * c);
	static Malla ** mallaAssimp(int &referenciaAObjID, std::string rutaAObj = "scr/GokuSSJ/GokuSSJ.obj", bool recalcular=true, bool simplificacion=true);
	~Malla();
	void initObj(int inPos, int inColor, int inNormal, int inTexCoord, int inTangent);
	void setModel(glm::mat4 nuevaModel);
	void renderizarMalla(glm::mat4 view, glm::mat4 proj, unsigned int IdModelView, unsigned int IdModelViewProj, unsigned int IdNormal);
	Malla* clone();
	int getNumCaras();
	int getVAO();
	void RecalcularNormalesYTangentes(bool simplificar=false);
};

