#include "Model3D.h"

#define GLM_FORCE_RADIANS
#include <math.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <vector>

#include <Assimp/color4.h>
#include <Assimp/mesh.h>
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>

unsigned int Model3D::numMeshes = 0;
Model3D::Model3D(const int numTriangles_, const int numVertices_, unsigned int* trianglesArray_, float* colorsArray_, float* normalArray_, float* positionsArray_, float* tangentArray_, float* textCoordArray_)
    : numTriangles(numTriangles_),
      id(numMeshes++),
      numVertices(numVertices_),
      trianglesArray(trianglesArray_),
      colorsArray(colorsArray_),
      normalsArray(normalArray_),
      positionsArray(positionsArray_),
      tangentsArray(tangentArray_),
      textCoordArray(textCoordArray_) {
    std::cout << "Created model: " << id << std::endl;
}

Model3D::Model3D(aiMesh* importedMesh)
    : id(numMeshes++) {
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Obtained model mesh:" << std::endl;

    numTriangles = importedMesh->mNumFaces;
    std::cout << "Mesh with " << numTriangles << " triangles." << std::endl;
    numVertices = importedMesh->mNumVertices;
    std::cout << "Mesh with" << numVertices << " vertices. Computing the array of triangles." << std::endl;

    if (importedMesh->HasFaces()) {
        trianglesArray = new unsigned int[numTriangles * 3];
        for (int i = 0; i < numTriangles; i++) {
            trianglesArray[3 * i] = importedMesh->mFaces[i].mIndices[0];
            trianglesArray[3 * i + 1] = importedMesh->mFaces[i].mIndices[1];
            trianglesArray[3 * i + 2] = importedMesh->mFaces[i].mIndices[2];
        }
    } else {
        std::cout << "ERROR: detected a mesh without triangles." << std::endl;
    }

    std::cout << "Obtained triangles array. Computing vertices positions." << std::endl;

    positionsArray = new float[numVertices * 3];
    for (int i = 0; i < numVertices; i++) {
        positionsArray[3 * i] = importedMesh->mVertices[i].x;
        positionsArray[3 * i + 1] = importedMesh->mVertices[i].y;
        positionsArray[3 * i + 2] = importedMesh->mVertices[i].z;
    }
    std::cout << "Obtained vertices positions. Computing vertices colors." << std::endl;

    if (importedMesh->HasVertexColors(0)) {
        colorsArray = new float[numVertices * 3];
        for (int i = 0; i < numVertices; i++) {
            colorsArray[3 * i] = importedMesh->mColors[0][i].r;
            colorsArray[3 * i + 1] = importedMesh->mColors[0][i].g;
            colorsArray[3 * i + 2] = importedMesh->mColors[0][i].b;
        }
    } else {
        std::cout << "WARNING: skipping vertex colors, the mesh does not contained them." << std::endl;
        colorsArray = nullptr;
    }

    std::cout << "Obtained color vertices. Computing vertices normals." << std::endl;

    if (importedMesh->HasNormals()) {
        normalsArray = new float[numVertices * 3];
        for (int i = 0; i < numVertices; i++) {
            normalsArray[3 * i] = importedMesh->mNormals[i].x;
            normalsArray[3 * i + 1] = importedMesh->mNormals[i].y;
            normalsArray[3 * i + 2] = importedMesh->mNormals[i].z;
        }
    } else {
        std::cout << "WARNING: skipping vertex normals, the mesh does not contained them." << std::endl;
        normalsArray = nullptr;
    }
    std::cout << "Obtained vertices normals. Computing vertices texture coordinates." << std::endl;

    if (importedMesh->HasTextureCoords(0)) {
        textCoordArray = new float[2 * numVertices];
        for (int i = 0; i < numVertices; i++) {
            textCoordArray[2 * i] = importedMesh->mTextureCoords[0][i].x;
            textCoordArray[2 * i + 1] = importedMesh->mTextureCoords[0][i].y;
        }
    } else {
        std::cout << "WARNING: skipping vertex texture coordinates, the mesh does not contained them." << std::endl;
        textCoordArray = nullptr;
    }
    std::cout << "Computed vertices texture coordinates. Computing vertices tangents." << std::endl;

    if (importedMesh->HasTangentsAndBitangents()) {
        float* tangentsArray = new float[numVertices * 3];
        for (int i = 0; i < numVertices; i++) {
            tangentsArray[3 * i] = importedMesh->mTangents[i].x;
            tangentsArray[3 * i + 1] = importedMesh->mTangents[i].y;
            tangentsArray[3 * i + 2] = importedMesh->mTangents[i].z;
        }
    } else {
        std::cout << "WARNING: skipping vertex tangents, the mesh does not contained them." << std::endl;
        tangentsArray = nullptr;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Model: " << id << " loaded in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
}

void Model3D::loadFromFile(std::vector<std::shared_ptr<Model3D>>& models, const std::string pathToFile) {
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\nStarting loading process. Please wait some seconds." << std::endl;
    Assimp::Importer importer;
    const aiScene* fileScene = importer.ReadFile(pathToFile, 0 /*Avoid postprocessing, Options to compute normalas/tangents, triangulate, optimize, etc.*/);

    int numMeshes = 0;
    if (!fileScene) {
        std::cout << (importer.GetErrorString());
    } else {
        std::cout << "Scene loaded from file. Computing number of meshes" << std::endl;
        numMeshes = fileScene->mNumMeshes;
        int numberModels = numMeshes + models.size();
        std::cout << "Loading scene with " << numMeshes << " meshes." << std::endl;
        models.reserve(numberModels);
        for (int i = 0; i < numMeshes; i++) {  //TODO optimize
            std::cout << std::endl
                      << std::endl
                      << std::endl
                      << "Loading mesh number " << i + 1 << ":" << std::endl
                      << std::endl;
            models.push_back(std::shared_ptr<Model3D>(new Model3D(fileScene->mMeshes[i])));
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "File with: " << numMeshes << " loaded in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
}

Model3D::~Model3D() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &positionVBO);  //Silently ignores 0 and not created buffers
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &textCoordVBO);
    glDeleteBuffers(1, &trianglesIndexVBO);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &modelVAO);
    std::cout << "Deleted mesh: " << id << std::endl;
}

void Model3D::initObject() {
    const auto start = std::chrono::high_resolution_clock::now();

    glGenVertexArrays(1, &modelVAO);
    glBindVertexArray(modelVAO);

    if (positionsArray) {
        glGenBuffers(1, &positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3 /*Buffer size*/, positionsArray, GL_STATIC_DRAW);
    }

    if (colorsArray) {
        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, colorsArray, GL_STATIC_DRAW);
    }

    if (normalsArray) {
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, normalsArray, GL_STATIC_DRAW);
    }

    if (textCoordArray) {
        glGenBuffers(1, &textCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 2, textCoordArray, GL_STATIC_DRAW);
    }

    if (tangentsArray) {
        glGenBuffers(1, &tangentVBO);
        glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, tangentsArray, GL_STATIC_DRAW);
    }

    glGenBuffers(1, &trianglesIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglesIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * sizeof(unsigned int) * 3, trianglesArray, GL_STATIC_DRAW);

    linkShaderVBOs();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Model inited in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
}

void Model3D::linkShaderVBOs() {
    glBindVertexArray(modelVAO);

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glVertexAttribPointer(0, 3 /*3 elements per vertex*/, GL_FLOAT, GL_FALSE, 0 /*stride*/, 0 /*offset*/);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, textCoordVBO);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(4);
}

void Model3D::renderModel() const {
    glBindVertexArray(modelVAO);
    glDrawElements(GL_TRIANGLES, numTriangles * 3, GL_UNSIGNED_INT, (void*)0 /* Offset of the first index in the array*/);
}

int Model3D::getNumTriangles() const {
    return numTriangles;
}

int Model3D::getVAO() const {
    return modelVAO;
}

void Model3D::recomputeNormalsAndTangents(const bool checkDuplicates) {
    const auto start = std::chrono::high_resolution_clock::now();
    glm::vec3* newTangetsArray = new glm::vec3[numVertices];
    glm::vec3* newNormalArray = new glm::vec3[numVertices];
    glm::vec3* newPositionArray = new glm::vec3[numVertices];
    std::cout << "Recomputing tangents and normals..." << std::endl;
    for (int i = 0; i < numVertices; i++) {
        newTangetsArray[i] = newNormalArray[i] = glm::vec3(0.0f);
        newPositionArray[i] = glm::vec3(positionsArray[3 * i], positionsArray[3 * i + 1], positionsArray[3 * i + 2]);
    }

    //I compute the normal for each triangle
    for (int i = 0; i < numTriangles; i++) {
        const unsigned int vertexId[3]{
            trianglesArray[3 * i],
            trianglesArray[3 * i + 1],
            trianglesArray[3 * i + 2]};
        glm::vec3 pointTangents[3]{
            newPositionArray[vertexId[1]] - newPositionArray[vertexId[0]],
            newPositionArray[vertexId[2]] - newPositionArray[vertexId[0]],
            newPositionArray[vertexId[2]] - newPositionArray[vertexId[1]]};

        //Of the infinite tangent vectors we use the ones following the texture coordinates.
        //If there is not texture coordinates we directly use the tangets

        float textVectorA = textCoordArray ? textCoordArray[2 * vertexId[1]] - textCoordArray[2 * vertexId[0]] : 1.0f;
        float textVectorB = textCoordArray ? textCoordArray[2 * vertexId[2]] - textCoordArray[2 * vertexId[0]] : 1.0f;

        glm::vec3 auxVector = glm::normalize(textVectorB * pointTangents[0] - textVectorA * pointTangents[1]);  //Formula from en mathematics for 3d Game Programing and computer graphics chapter 7.8

        for (int a = 0; a < 3; ++a) {
            pointTangents[a] = glm::normalize(pointTangents[a]);
        }
        //We use the angle of each vertex to weight the vectors
        newTangetsArray[vertexId[0]] += glm::angle(pointTangents[0], pointTangents[1]) * auxVector;
        newTangetsArray[vertexId[1]] += glm::angle(pointTangents[2], -pointTangents[0]) * auxVector;
        newTangetsArray[vertexId[2]] += glm::angle(pointTangents[2], pointTangents[1]) * auxVector;

        //We reuse auxVector to compute the normals.
        auxVector = glm::normalize(glm::cross(pointTangents[0], pointTangents[1]));

        newNormalArray[vertexId[0]] += glm::angle(pointTangents[0], pointTangents[1]) * auxVector;
        newNormalArray[vertexId[1]] += glm::angle(pointTangents[2], -pointTangents[0]) * auxVector;
        newNormalArray[vertexId[2]] += glm::angle(pointTangents[2], pointTangents[1]) * auxVector;
    }

    if (checkDuplicates) {
        int checkedVertices = 0;
        glm::vec3* tempPositionArray = new glm::vec3[numVertices];
        glm::vec3* tempNormalArray = new glm::vec3[numVertices];
        glm::vec3* tempTangentArray = new glm::vec3[numVertices];

        for (int i = 0; i < numVertices; i++) {
            bool isDuplicated = false;
            for (int m = 0; m < checkedVertices && !isDuplicated; m++) {
                if (newPositionArray[i] == tempPositionArray[m]) {
                    newNormalArray[i] = tempNormalArray[m];
                    newTangetsArray[i] = tempTangentArray[m];
                    isDuplicated = true;
                }
            }
            if (!isDuplicated) {
                for (int n = i + 1; n < numVertices; n++) {
                    if (newPositionArray[n] == newPositionArray[i]) {
                        newNormalArray[i] += newNormalArray[n];
                        newTangetsArray[i] += newTangetsArray[n];
                    }
                }
                tempPositionArray[checkedVertices] = newPositionArray[i];
                tempNormalArray[checkedVertices] = newNormalArray[i];
                tempTangentArray[checkedVertices] = newTangetsArray[i];
                ++checkedVertices;
            }
        }
        delete[] tempPositionArray;
        delete[] tempNormalArray;
        delete[] tempTangentArray;
    }

    delete[] newPositionArray;
    delete[] normalsArray;
    delete[] tangentsArray;
    normalsArray = new float[numVertices * 3];
    tangentsArray = new float[numVertices * 3];

    const double maxValue = 1.07 * glm::half_pi<double>();
    const double minValue = 0.93 * glm::half_pi<double>();
    for (int i = 0; i < numVertices; i++) {
        newNormalArray[i] = glm::normalize(newNormalArray[i]);
        newTangetsArray[i] = glm::normalize(newTangetsArray[i] - newNormalArray[i] * glm::dot(newNormalArray[i], newTangetsArray[i]));  //Gram-Schmidt process

        //Check normal and tangent are perpendicular
        if (glm::angle(newNormalArray[i], newTangetsArray[i]) > maxValue || glm::angle(newNormalArray[i], newTangetsArray[i]) < minValue) {
            std::cout << "WARNING on vertex: " << i << ": detected angle of " << glm::angle(newNormalArray[i], newTangetsArray[i]) << " between normal and tangent" << std::endl;
        }

        for (int a = 0; a < 3; ++a) {
            normalsArray[3 * i + a] = newNormalArray[i][a];
            tangentsArray[3 * i + a] = newTangetsArray[i][a];
        }
    }
    delete[] newTangetsArray;
    delete[] newNormalArray;
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Normals and tangents recomputed in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
}

void Model3D::combineSimilarVertices(const bool checkColorAndTextures) {
    const auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Simplifying mesh with: " << numVertices << " vertices." << std::endl;
    std::vector<glm::vec3> tempPositions;
    std::vector<float> tempNormals;
    std::vector<float> tempTangents;
    std::vector<glm::vec3> tempColor;
    std::vector<glm::vec2> tempTextCoord;
    const int numTriangleVertices = 3 * numTriangles;

    int notUniques = -1;
    for (int i = 0; i < numVertices; i++) {
        bool uniqueVertex = true;
        glm::vec3 actualVertexPosition(positionsArray[3 * i], positionsArray[3 * i + 1], positionsArray[3 * i + 2]);
        glm::vec3 actualVertexColor;
        glm::vec2 actualVertexTexture;
        if (colorsArray) {
            actualVertexColor = glm::vec3(colorsArray[3 * i], colorsArray[3 * i + 1], colorsArray[3 * i + 2]);
        }
        if (textCoordArray) {
            actualVertexTexture = glm::vec2(textCoordArray[2 * i], textCoordArray[2 * i + 1]);
        }

        int vertexPosition = -1;

        for (int a = 0, length = tempPositions.size(); a < length && uniqueVertex; ++a) {
            if (actualVertexPosition == tempPositions.at(a)) {
                if (checkColorAndTextures && (!colorsArray || !textCoordArray)) {
                    if (colorsArray && actualVertexColor == tempColor.at(a)) {
                        uniqueVertex = false;
                    }
                    if (textCoordArray && actualVertexTexture == tempTextCoord.at(a)) {
                        uniqueVertex = false;
                    }
                } else {
                    uniqueVertex = false;
                }
            }
            ++vertexPosition;
        }
        if (uniqueVertex) {
            tempPositions.push_back(actualVertexPosition);
            if (normalsArray) {
                tempNormals.push_back(normalsArray[3 * i]);
                tempNormals.push_back(normalsArray[3 * i + 1]);
                tempNormals.push_back(normalsArray[3 * i + 2]);
            }
            if (tangentsArray) {
                tempTangents.push_back(tangentsArray[3 * i]);
                tempTangents.push_back(tangentsArray[3 * i + 1]);
                tempTangents.push_back(tangentsArray[3 * i + 2]);
            }
            if (colorsArray) {
                tempColor.push_back(actualVertexColor);
            }
            if (textCoordArray) {
                tempTextCoord.push_back(actualVertexTexture);
            }
        } else {
            ++notUniques;
            for (int triangleVertex = 0; triangleVertex < numTriangleVertices; ++triangleVertex) {
                if (trianglesArray[triangleVertex] == (i - notUniques)) {
                    trianglesArray[triangleVertex] = vertexPosition;
                } else if (static_cast<int>(trianglesArray[triangleVertex]) > (i - notUniques)) {
                    trianglesArray[triangleVertex] = trianglesArray[triangleVertex] - 1;
                }
            }
        }
    }
    int oldNumVertices = numVertices;
    numVertices = tempPositions.size();
    delete[] positionsArray;
    positionsArray = new float[numVertices * 3];
    int vertexId = 0;
    for (auto it = tempPositions.begin(); it != tempPositions.end(); ++it) {
        positionsArray[vertexId++] = it->x;
        positionsArray[vertexId++] = it->y;
        positionsArray[vertexId++] = it->z;
    }
    if (normalsArray) {
        delete[] normalsArray;
        normalsArray = new float[numVertices * 3];
        int vertexId = 0;
        for (auto it = tempNormals.begin(); it != tempNormals.end(); ++it) {
            normalsArray[vertexId++] = *it;
        }
    }
    if (tangentsArray) {
        delete[] tangentsArray;
        tangentsArray = new float[numVertices * 3];
        int vertexId = 0;
        for (auto it = tempTangents.begin(); it != tempTangents.end(); ++it) {
            tangentsArray[vertexId++] = *it;
        }
    }
    if (colorsArray) {
        delete[] colorsArray;
        colorsArray = new float[numVertices * 3];
        int vertexId = 0;
        for (auto it = tempColor.begin(); it != tempColor.end(); ++it) {
            colorsArray[vertexId++] = it->x;
            colorsArray[vertexId++] = it->y;
            colorsArray[vertexId++] = it->z;
        }
    }
    if (textCoordArray) {
        delete[] textCoordArray;
        textCoordArray = new float[numVertices * 2];
        int vertexId = 0;
        for (auto it = tempTextCoord.begin(); it != tempTextCoord.end(); ++it) {
            textCoordArray[vertexId++] = it->x;
            textCoordArray[vertexId++] = it->y;
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Completed model simplification from " << oldNumVertices << " vertices to " << numVertices << " vertices in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;
}

Model3D* Model3D::allocateModel3D(const int numTriangles_, const int numVertices_, const unsigned int* trianglesArray_, const float* colorsArray_, const float* normalArray_, const float* positionsArray_, const float* tangentArray_, const float* textCoordArray_) {
    const auto start = std::chrono::high_resolution_clock::now();

    unsigned int* trianglesArrayAux = new unsigned int[3 * numTriangles_];
    float* colorsArrayAux = new float[3 * numVertices_];
    float* normalsArrayAux = new float[3 * numVertices_];
    float* positionsArrayAux = new float[3 * numVertices_];
    float* tangentsArrayAux = new float[3 * numVertices_];
    float* textCoordArrayAux = new float[2 * numVertices_];
    for (int i = 0; i < 3 * numTriangles_; ++i) {
        trianglesArrayAux[i] = trianglesArray_[i];
    }
    for (int i = 0, length = 3 * numVertices_; i < length; ++i) {
        colorsArrayAux[i] = colorsArray_[i];
        normalsArrayAux[i] = normalArray_[i];
        positionsArrayAux[i] = positionsArray_[i];
        tangentsArrayAux[i] = tangentArray_[i];
    }
    for (int i = 0, length = 2 * numVertices_; i < length; ++i) {
        textCoordArrayAux[i] = textCoordArray_[i];
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Model allocated in: " << timeDiff << " nanoSeconds = " << 1e-9f * static_cast<float>(timeDiff) << " seconds." << std::endl;

    return new Model3D(numTriangles_, numVertices_, trianglesArrayAux, colorsArrayAux, normalsArrayAux, positionsArrayAux, tangentsArrayAux, textCoordArrayAux);
}

void Model3D::removeNormals() {
    delete[] normalsArray;
    normalsArray = nullptr;
}
void Model3D::removeTangents() {
    delete[] tangentsArray;
    tangentsArray = nullptr;
}
void Model3D::removeTextCoords() {
    delete[] textCoordArray;
    textCoordArray = nullptr;
}
void Model3D::removeColors() {
    delete[] colorsArray;
    colorsArray = nullptr;
}