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
      struct VertexData {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 color;
        glm::vec2 textCoord;
    };

    VertexData* verticesArray = nullptr;

    GLuint modelVAO = 0;
    GLuint positionVBO = 0;
    GLuint colorVBO = 0;
    GLuint normalVBO = 0;
    GLuint tangentVBO = 0;
    GLuint textCoordVBO = 0;
    GLuint trianglesIndexVBO = 0;

    int numTriangles = 0;
    int numVertices = 0;
    unsigned int *trianglesArray = nullptr;
    float *positionsArray = nullptr;
    float *normalsArray = nullptr;
    float *tangentsArray = nullptr;
    float *colorsArray = nullptr;
    float *textCoordArray = nullptr;


    glm::mat4 modelMatrix = glm::mat4(1.0f);

    static Model3D* allocateModel3D(const int numTriangles_, const int numVertices_, const unsigned int* trianglesArray_ = nullptr, const float* colorsArray_ = nullptr, const float* normalArray_ = nullptr, const float* positionsArray_ = nullptr, const float* tangentArray_ = nullptr, const float* textCoordArray_ = nullptr);

   public:
    Model3D(const int numTriangles_, const int numVertices_, unsigned int* trianglesArray_ = nullptr, float* colorsArray_ = nullptr, float* normalArray_ = nullptr, float* positionsArray_ = nullptr, float* tangentArray_ = nullptr, float* textCoordArray_ = nullptr);
    Model3D(aiMesh *c);
    static Model3D **loadFromFile(int& numberModels, std::string pathToFile);
    ~Model3D();
    void initObject();
    void linkShaderVBOs(GLint inPos = -1, GLint inColor = -1, GLint inNormal = -1, GLint inTexCoord = -1, GLint inTangent = -1);
    void renderModel(glm::mat4 view, glm::mat4 proj, GLint IdModelView = -1, GLint IdModelViewProj = -1, GLint IdNormal = -1);
    Model3D *clone();
    void recomputeNormalsAndTangents(bool checkDuplicates = true);
    void combineSimilarVertices(const bool checkColorAndTextures = true);

    int getNumTriangles();
    int getVAO();

    void setModel(glm::mat4 modelMatrix_);
    void removeNormals();
    void removeTangents();
    void removeTextCoords();
    void removeColors();

    static Model3D* getCubeModel();
};
#endif  //RENDER_MOTOR_MODEL_3D_H