#include "BatchedRenderer.h"

#include <cstdint>

#include "shader/binary/default.vs.h"
#include "shader/binary/default.fs.h"

BatchedRenderer* BatchedRenderer::batchedRendererInstance = nullptr;

void BatchedRenderer::draw() {
    {
        glClear(GL_COLOR_BUFFER_BIT);
        if(egl_initialised){
            DisplayManager::updateDisplay();
        }
    }
}

void BatchedRenderer::onWindowInit() {
    AppStateManager::onWindowInit();
    DisplayManager::init(getAppContext());
    egl_initialised = true;
    // Renderer initialise here

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    shader = new Shader();

    shader->addShader(default_vs.data(), GL_VERTEX_SHADER);
    shader->addShader(default_fs.data(), GL_FRAGMENT_SHADER);
    shader->buildProgram();
    shader->useProgram();
}

void BatchedRenderer::onWindowTerminate() {
    AppStateManager::onWindowTerminate();
}

void BatchedRenderer::init(android_app *pApp) {
    if(batchedRendererInstance == nullptr) batchedRendererInstance = new BatchedRenderer();
    batchedRendererInstance->setAppContext(pApp);
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
