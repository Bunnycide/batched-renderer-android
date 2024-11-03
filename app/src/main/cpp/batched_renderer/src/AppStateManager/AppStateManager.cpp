#include "AppStateManager/AppStateManager.h"
#include "../../AndroidOut.h"

AppStateManager::AppStateManager(android_app *pApp_) : android_app_context(pApp_) { }

void AppStateManager::loop() {
    do{
        pollingDone = false;

        while(!pollingDone){
            result = ALooper_pollOnce(timeout, nullptr, &events, reinterpret_cast<void **>(&pSource));

            switch (result) {
                case ALOOPER_POLL_TIMEOUT:
                case ALOOPER_POLL_WAKE:
                    // No events occurred before the timeout or explicit wake. Stop checking for events
                    pollingDone = true;
                    break;
                case ALOOPER_POLL_ERROR:
                    aout << "ALooper_pollOnce returned an error" << std::endl;
                    break;
                case ALOOPER_POLL_CALLBACK:
                    break;
                default:
                    if(pSource){
                        pSource->process(android_app_context, pSource);
                    }
            }
        }

        draw();

    } while (! android_app_context->destroyRequested);
}

/**
 * @brief When window is first initialised this member is called
 * Take care of any initialisation procedures that depend on a window surface in this function
 * */
void AppStateManager::onWindowTerminate() {

}

/**
 * @brief When window associated with the app is terminated this is called
 * Take care of any de-initialisation here
 * */
void AppStateManager::onWindowInit() {

}

/**
 * @brief Depending on the passed app state calls the appropriate handlers
 *
 * */
void AppStateManager::handleAppState(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:{
            this->onWindowInit();
        }
            break;
        case APP_CMD_TERM_WINDOW:{
            this->onWindowTerminate();
        }
            break;
        default:
            break;
    }
}

void AppStateManager::setAppContext(android_app *pApp) {
    android_app_context = pApp;
}

android_app *AppStateManager::getAppContext() {
    return this->android_app_context;
}
