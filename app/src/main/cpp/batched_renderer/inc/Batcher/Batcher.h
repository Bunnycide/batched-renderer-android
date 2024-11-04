#ifndef BATCHED_GEOMETRY_GENERATOR_
#define BATCHED_GEOMETRY_GENERATOR_

#include "glm/glm.hpp"

typedef struct VertexData_{
    glm::vec4 vertexPosition;
    glm::vec3 vertexNormal;
    glm::vec2 texCoordinates;

    [[nodiscard]] static unsigned int setupVAO(const unsigned int&);

} VertexData;

typedef struct Mesh_{
    unsigned int vao, ebo;
    std::vector<VertexData> meshData;
    std::vector<unsigned int> faces;
} Mesh;

class BatchedGeometryGenerator {
public:
    BatchedGeometryGenerator();
    BatchedGeometryGenerator* addGeometry(Mesh* );
    Mesh* getBatchedData();
private:
    Mesh* batchedMeshData;
};

#endif // BATCHED_GEOMETRY_GENERATOR_
