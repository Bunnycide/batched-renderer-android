#include "BatchedRenderer.h"

#include <cstdint>

#include "shader/binary/default.vs.h"
#include "shader/binary/default.fs.h"
#include "Batcher/Batcher.h"

#include <glm/gtc/matrix_transform.hpp>
#include "ClusterGenerator/cluster-generator.h"


unsigned int VertexData_::setupVAO(const unsigned int& vbo) {
    unsigned int vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (VertexData), (void*)offsetof(VertexData, vertexPosition));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (VertexData), (void*)offsetof(VertexData, vertexNormal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (VertexData), (void*)offsetof(VertexData, texCoordinates));
    glEnableVertexAttribArray(2);

    return vao;
}

BatchedRenderer* BatchedRenderer::batchedRendererInstance = nullptr;

void BatchedRenderer::draw() {
    {
        if(egl_initialised){
            shader->useProgram();

            glClear(GL_COLOR_BUFFER_BIT);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchedMesh->ebo);
            glBindVertexArray(batchedMesh->vao);

            glDrawElements(GL_TRIANGLES, batchedMesh->faces.size(), GL_UNSIGNED_INT, nullptr);
            DisplayManager::updateDisplay();
        }
    }
}

void BatchedRenderer::onWindowInit() {
    AppStateManager::onWindowInit();
    DisplayManager::init(getAppContext());

    // Renderer initialise here

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    shader = new Shader();
    camera = new Camera();

    shader->addShader(default_vs.data(), GL_VERTEX_SHADER);
    shader->addShader(default_fs.data(), GL_FRAGMENT_SHADER);
    shader->buildProgram();

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER,  vbo);
    glBufferData(GL_ARRAY_BUFFER, batchedMesh->meshData.size() * sizeof(VertexData), batchedMesh->meshData.data(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &batchedMesh->ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchedMesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchedMesh->faces.size() * sizeof (unsigned int), batchedMesh->faces.data(), GL_DYNAMIC_DRAW);

    // Before we draw bind the vertex buffer and the index buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchedMesh->ebo);

    // vao
    batchedMesh->vao = VertexData ::setupVAO(vbo);

    // Projection matrix

    glm::mat4 proj(1), view(1), model(1);

    proj = glm::perspective(glm::radians(45.0f),
                            (float)DisplayManager::getWidth()/(float)DisplayManager::getHeight(),
                            0.1f, 100.f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
    model = glm::scale(model, glm::vec3 (0.125f, 0.125f, 1.0f));

    shader->useProgram();

    shader->set("u_proj", proj);
    shader->set("u_view", camera->GetViewMatrix());

    shader->set("u_model", model);

    egl_initialised = true;
}

void BatchedRenderer::onWindowTerminate() {
    AppStateManager::onWindowTerminate();
}

void BatchedRenderer::init(android_app *pApp) {
    if(batchedRendererInstance == nullptr) batchedRendererInstance = new BatchedRenderer();
    batchedRendererInstance->setAppContext(pApp);

    float x[2]  = {2.0f, -2.0f};

    for(int j =0; j < 2; j++){
        // New dodecahedron object
        batchedRendererInstance->dodecahedron[j] = new Mesh();
        batchedRendererInstance->batchedGeometryGenerator = new BatchedGeometryGenerator();

        // Create a dodecahedra
        generate_dodecahedra( batchedRendererInstance->mesh[j], 1.0f, {x[j], 0.0f, 0.0f});

        // Increase the LOD level for cluster 0
        increaseLODForClusterGroup( batchedRendererInstance->mesh[j], 0);

        // TODO: Implement the decrease LOD level func
        // Stop doing this and move this somewhere else
        batchedRendererInstance->dodecahedron[j]->meshData.resize(batchedRendererInstance->mesh[j].vertices.size());

        for(int i = 0; i < batchedRendererInstance->mesh[j].vertices.size(); i++) {
            batchedRendererInstance->dodecahedron[j]->meshData[i].vertexPosition = batchedRendererInstance->mesh[j].vertices[i];
            batchedRendererInstance->dodecahedron[j]->meshData[i].vertexNormal = {0.0f, 0.0f, 0.0f};
            batchedRendererInstance->dodecahedron[j]->meshData[i].texCoordinates = {0.0f, 0.0f};
        }

        batchedRendererInstance->dodecahedron[j]->faces.clear();

        // Copy over the indices

        for(auto& clusterGroup : batchedRendererInstance->mesh[j].clusterGroups){
            for(auto& cluster : clusterGroup.clusters){
                batchedRendererInstance->dodecahedron[j]->faces.insert(
                        batchedRendererInstance->dodecahedron[j]->faces.end(),
                        cluster.indices.begin(),
                        cluster.indices.end());
            }
        }

        batchedRendererInstance->batchedGeometryGenerator->addGeometry(batchedRendererInstance->dodecahedron[j]);
    }

    batchedRendererInstance->batchedMesh = batchedRendererInstance->batchedGeometryGenerator->getBatchedData();

    aout << "Number of indices " << batchedRendererInstance->batchedMesh->faces.size() << std::endl;
}

void BatchedRenderer::handle_app_state(int32_t cmd) {
    batchedRendererInstance->handleAppState(cmd);
}

void BatchedRenderer::main_loop(android_app *pApp) {
    if(batchedRendererInstance != nullptr){
        batchedRendererInstance->setAppContext(pApp);
        batchedRendererInstance->loop();
    }

    else aout << "Renderer not initialised" << std::endl;
}


