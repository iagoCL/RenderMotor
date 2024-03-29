#include "Model3D.h"

Model3D* Model3D::getCubeModel() {
    const int cubeNVertex = 24;         // 4 vertices x 6 faces
    const int cubeNTriangleIndex = 12;  // 2 triangles x  6 face;

    const unsigned int cubeTriangleIndex[] = {
        0, 1, 2, 1, 3, 2,        // Face z = 1
        4, 6, 5, 5, 6, 7,        // Face z = -1
        8, 10, 9, 9, 10, 11,     // Face x = 1
        12, 13, 14, 13, 15, 14,  // Face x = -1
        16, 17, 18, 17, 19, 18,  // Face y = 1
        20, 22, 21, 21, 22, 23,  // Face y = -1
    };

    // Vertices positions
    const float cubeVertexPos[] = {
        // Face z = 1
        -1.0f, -1.0f, 1.0f,  // 0
        1.0f, -1.0f, 1.0f,   // 1
        -1.0f, 1.0f, 1.0f,   // 2
        1.0f, 1.0f, 1.0f,    // 3
        // Face z = -1
        -1.0f, -1.0f, -1.0f,  // 4
        1.0f, -1.0f, -1.0f,   // 5
        -1.0f, 1.0f, -1.0f,   // 6
        1.0f, 1.0f, -1.0f,    // 7
        // Face x = 1
        1.0f, -1.0f, -1.0f,  // 8
        1.0f, -1.0f, 1.0f,   // 9
        1.0f, 1.0f, -1.0f,   // 10
        1.0f, 1.0f, 1.0f,    // 11
        // Face x = -1
        -1.0f, -1.0f, -1.0f,  // 12
        -1.0f, -1.0f, 1.0f,   // 13
        -1.0f, 1.0f, -1.0f,   // 14
        -1.0f, 1.0f, 1.0f,    // 15
        // Face y = 1
        -1.0f, 1.0f, -1.0f,  // 16
        -1.0f, 1.0f, 1.0f,   // 17
        1.0f, 1.0f, -1.0f,   // 18
        1.0f, 1.0f, 1.0f,    // 19
        // Face y = -1
        -1.0f, -1.0f, -1.0f,  // 20
        -1.0f, -1.0f, 1.0f,   // 21
        1.0f, -1.0f, -1.0f,   // 22
        1.0f, -1.0f, 1.0f     // 23
    };

    // Normal de los vertices
    const float cubeVertexNormal[] = {
        // Face z = 1
        0.0f, 0.0f, 1.0f,  // 0
        0.0f, 0.0f, 1.0f,  // 1
        0.0f, 0.0f, 1.0f,  // 2
        0.0f, 0.0f, 1.0f,  // 3
        // Face z = -1
        0.0f, 0.0f, -1.0f,  // 4
        0.0f, 0.0f, -1.0f,  // 5
        0.0f, 0.0f, -1.0f,  // 6
        0.0f, 0.0f, -1.0f,  // 7
        // Face x = 1
        1.0f, 0.0f, 0.0f,  // 8
        1.0f, 0.0f, 0.0f,  // 9
        1.0f, 0.0f, 0.0f,  // 10
        1.0f, 0.0f, 0.0f,  // 11
        // Face x = -1
        -1.0f, 0.0f, 0.0f,  // 12
        -1.0f, 0.0f, 0.0f,  // 13
        -1.0f, 0.0f, 0.0f,  // 14
        -1.0f, 0.0f, 0.0f,  // 15
        // Face y = 1
        0.0f, 1.0f, 0.0f,  // 16
        0.0f, 1.0f, 0.0f,  // 17
        0.0f, 1.0f, 0.0f,  // 18
        0.0f, 1.0f, 0.0f,  // 19
        // Face y = -1
        0.0f, -1.0f, 0.0f,  // 20
        0.0f, -1.0f, 0.0f,  // 21
        0.0f, -1.0f, 0.0f,  // 22
        0.0f, -1.0f, 0.0f   // 23
    };

    // Color de los vertices
    const float cubeVertexColor[] = {
        // Face z = 1
        0.0f, 0.0f, 1.0f,  // 0
        0.0f, 0.0f, 1.0f,  // 1
        0.0f, 0.0f, 1.0f,  // 2
        0.0f, 0.0f, 1.0f,  // 3
        // Face z = -1
        0.6f, 0.6f, 1.0f,  // 4
        0.6f, 0.6f, 1.0f,  // 5
        0.6f, 0.6f, 1.0f,  // 6
        0.6f, 0.6f, 1.0f,  // 7
        // Face x = 1
        1.0f, 0.0f, 0.0f,  // 8
        1.0f, 0.0f, 0.0f,  // 9
        1.0f, 0.0f, 0.0f,  // 10
        1.0f, 0.0f, 0.0f,  // 11
        // Face x = -1
        1.0f, 0.6f, 0.6f,  // 12
        1.0f, 0.6f, 0.6f,  // 13
        1.0f, 0.6f, 0.6f,  // 14
        1.0f, 0.6f, 0.6f,  // 15
        // Face y = 1
        0.0f, 1.0f, 0.0f,  // 16
        0.0f, 1.0f, 0.0f,  // 17
        0.0f, 1.0f, 0.0f,  // 18
        0.0f, 1.0f, 0.0f,  // 19
        // Face y = -1
        0.6f, 1.0f, 0.6f,  // 20
        0.6f, 1.0f, 0.6f,  // 21
        0.6f, 1.0f, 0.6f,  // 22
        0.6f, 1.0f, 0.6f   // 23
    };

    // Vertices texture coord.
    const float cubeVertexTexCoord[] = {
        // Face z = 1
        0.0f, 0.0f,  // 0
        1.0f, 0.0f,  // 1
        0.0f, 1.0f,  // 2
        1.0f, 1.0f,  // 3
        // Face z = -1
        0.0f, 1.0f,  // 4
        1.0f, 1.0f,  // 5
        0.0f, 0.0f,  // 6
        1.0f, 0.0f,  // 7
        // Face x = 1
        0.0f, 1.0f,  // 8
        1.0f, 1.0f,  // 9
        0.0f, 0.0f,  // 10
        1.0f, 0.0f,  // 11
        // Face x = -1
        0.0f, 0.0f,  // 12
        1.0f, 0.0f,  // 13
        0.0f, 1.0f,  // 14
        1.0f, 1.0f,  // 15
        // Face y = 1
        0.0f, 1.0f,  // 16
        0.0f, 0.0f,  // 17
        1.0f, 1.0f,  // 18
        1.0f, 0.0f,  // 19
        // Face y = -1
        0.0f, 0.0f,  // 20
        0.0f, 1.0f,  // 21
        1.0f, 0.0f,  // 22
        1.0f, 1.0f   // 23
    };

    const float cubeVertexTangent[] = {
        // Face z = 1
        1.0f, 0.0f, 0.0f,  // 0
        1.0f, 0.0f, 0.0f,  // 1
        1.0f, 0.0f, 0.0f,  // 2
        1.0f, 0.0f, 0.0f,  // 3
        // Face z = -1
        1.0f, 0.0f, 0.0f,  // 4
        1.0f, 0.0f, 0.0f,  // 5
        1.0f, 0.0f, 0.0f,  // 6
        1.0f, 0.0f, 0.0f,  // 7
        // Face x = 1
        0.0f, 0.0f, -1.0f,  // 8
        0.0f, 0.0f, -1.0f,  // 9
        0.0f, 0.0f, -1.0f,  // 10
        0.0f, 0.0f, -1.0f,  // 11
        // Face x = -1
        0.0f, 0.0f, 1.0f,  // 12
        0.0f, 0.0f, 1.0f,  // 13
        0.0f, 0.0f, 1.0f,  // 14
        0.0f, 0.0f, 1.0f,  // 15
        // Face y = 1
        1.0f, 0.0f, 0.0f,  // 16
        1.0f, 0.0f, 0.0f,  // 17
        1.0f, 0.0f, 0.0f,  // 18
        1.0f, 0.0f, 0.0f,  // 19
        // Face y = -1
        1.0f, 0.0f, 0.0f,  // 20
        1.0f, 0.0f, 0.0f,  // 21
        1.0f, 0.0f, 0.0f,  // 22
        1.0f, 0.0f, 0.0f   // 23
    };
    return allocateModel3D(cubeNTriangleIndex, cubeNVertex, &(cubeTriangleIndex[0]), &(cubeVertexColor[0]), &(cubeVertexNormal[0]), &(cubeVertexPos[0]), &(cubeVertexTangent[0]), &(cubeVertexTexCoord[0]));
}