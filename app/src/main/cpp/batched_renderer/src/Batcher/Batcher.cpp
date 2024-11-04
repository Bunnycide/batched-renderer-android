#include "Batcher/Batcher.h"
#include "../../../AndroidOut.h"

BatchedGeometryGenerator* BatchedGeometryGenerator::addGeometry(Mesh *mesh) {
    // Batch the vertex data together
    batchedMeshData->meshData.insert(batchedMeshData->meshData.end(), mesh->meshData.begin(), mesh->meshData.end());
    auto offset = batchedMeshData->faces.empty() ? 0 : batchedMeshData->faces.size();

    // Batch the index data together
    for(auto i = 0; i < mesh->faces.size(); i++){
        batchedMeshData->faces.push_back(mesh->faces[i] + offset);
    }

    return this;
}

BatchedGeometryGenerator::BatchedGeometryGenerator() {
    batchedMeshData = new Mesh();
}

Mesh *BatchedGeometryGenerator::getBatchedData() {
    return batchedMeshData;
}
