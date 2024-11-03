#ifndef BATCHED_RENDERER_
#define BATCHED_RENDERER_

#include "AppStateManager/AppStateManager.h"
#include "GLES3/gl3.h"
#include "DisplayManager/DisplayManager.h"
#include "../AndroidOut.h"
#include "Shader/Shader.h"


class BatchedRenderer : public AppStateManager {
public:
    static void handle_app_state(int32_t cmd);

    static void init(android_app* pApp);

    static void main_loop(android_app* pApp);

private:
    Shader* shader;

    BatchedRenderer() = default;

    static BatchedRenderer* batchedRendererInstance;

    bool egl_initialised = false;

    void draw() override;

    void onWindowInit() override;

    void onWindowTerminate() override;


};

#endif