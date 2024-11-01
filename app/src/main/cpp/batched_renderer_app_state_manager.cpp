#include "batched_renderer_app_state_manager.h"
#include "DisplayManager.h"
#include "AppStateManager.h"
#include "AndroidOut.h"

BatchedRendererApp* BatchedRendererApp::batchedRendererApp = nullptr;

void BatchedRendererApp::updateFrame(android_app* pApp) {
    batchedRendererApp->current_app_context = pApp;
    // Call the root method
    batchedRendererApp->updateFrame_();

    do{
        while(! batchedRendererApp->done){
            aout << "Updating" << std::endl;

            batchedRendererApp->inputPollResult =
                    ALooper_pollOnce(batchedRendererApp->timeout, nullptr, &batchedRendererApp->events,
                                     reinterpret_cast<void**>(&batchedRendererApp->pSource));

            switch (batchedRendererApp->inputPollResult) {
                case ALOOPER_POLL_TIMEOUT:
                    [[clang::fallthrough]];
                case ALOOPER_POLL_WAKE:
//                    batchedRendererApp->done = true;
                    break;
                case ALOOPER_EVENT_ERROR:
                    aout << "Error in the event looper" << std::endl;
                    break;
                case ALOOPER_POLL_CALLBACK:
                    break;
                default:
                    if(batchedRendererApp->pSource){
                        // Pump the events
                        batchedRendererApp->pSource->process(batchedRendererApp->current_app_context, batchedRendererApp->pSource);
                    }
            }
        }

    } while (!batchedRendererApp->current_app_context->destroyRequested);

    // TODO:(@JaeB) Add draw command imp here
}

void BatchedRendererApp::updateFrame_() {
    // Call the root method
    AppStateManager::updateFrame_();
    // Called when frame needs to be updated
    // Swap frame etc..

}

void BatchedRendererApp::onAppInit_() {
    // Call the root method
    AppStateManager::onAppInit_();
}

void BatchedRendererApp::onWindowTerminate_() {
    // Call the root method
    AppStateManager::onWindowTerminate_();
    // Called when window is about to be terminated
    aout << "App window terminated 2" << std::endl;
}

BatchedRendererApp::BatchedRendererApp(android_app *pApp) {
    setCurrentAppContext(pApp);
}

void BatchedRendererApp::updateCmd(int32_t cmd, android_app* pApp) {
    batchedRendererApp->AppStateManager::updateCmd(cmd, pApp);
}

void BatchedRendererApp::init(android_app *pApp) {
    if(batchedRendererApp == nullptr){
        batchedRendererApp = new BatchedRendererApp(pApp);
    }

    aout << "Initialised batched renderer" << std::endl;
}

