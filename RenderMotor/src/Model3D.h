#ifndef RENDER_MOTOR_MODEL_3D_H
#define RENDER_MOTOR_MODEL_3D_H

#include <gl/glew.h>

#include <gl/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <Assimp/color4.h>
#include <Assimp/mesh.h>
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>

class Model3D {
   private:
    GLuint modelVAO = 0;
    GLuint positionVBO = 0;
    GLuint colorVBO = 0;
    GLuint normalVBO = 0;
    GLuint tangentVBO = 0;
    GLuint textCoordVBO = 0;
    GLuint trianglesIndexVBO = 0;

    int numTriangles = 0;
    int numVertices = 0;
    unsigned int* trianglesArray = nullptr;
    float* positionsArray = nullptr;
    float* normalsArray = nullptr;
    float* tangentsArray = nullptr;
    float* colorsArray = nullptr;
    float* textCoordArray = nullptr;

    static Model3D* allocateModel3D(const int numTriangles_, const int numVertices_, const unsigned int* trianglesArray_ = nullptr, const float* colorsArray_ = nullptr, const float* normalArray_ = nullptr, const float* positionsArray_ = nullptr, const float* tangentArray_ = nullptr, const float* textCoordArray_ = nullptr);
    void linkShaderVBOs();

   public:
    Model3D(const int numTriangles_, const int numVertices_, unsigned int* trianglesArray_ = nullptr, float* colorsArray_ = nullptr, float* normalArray_ = nullptr, float* positionsArray_ = nullptr, float* tangentArray_ = nullptr, float* textCoordArray_ = nullptr);
    Model3D(aiMesh* assimpMesh);
    static Model3D** loadFromFile(int& numberModels, const std::string pathToFile);
    ~Model3D();
    void initObject();
    void renderModel() const;
    void recomputeNormalsAndTangents(const bool checkDuplicates = true);
    void combineSimilarVertices(const bool checkColorAndTextures = true);

    int getNumTriangles() const;
    int getVAO() const;

    void removeNormals();
    void removeTangents();
    void removeTextCoords();
    void removeColors();

    static Model3D* getCubeModel();
};
#endif  //RENDER_MOTOR_MODEL_3D_H