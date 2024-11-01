#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "AppStateManager.h"
#include "../../AndroidOut.h"
#include "DisplayManager.h"

void AppStateManager::onWindowTerminate_() {

}

void AppStateManager::onAppInit_() {
    DisplayManager::init(current_app_context);
}

void AppStateManager::loop() {

}

void AppStateManager::updateFrame_() {

}

void AppStateManager::updateCmd(int32_t cmd, android_app* pApp) {
    setCurrentAppContext(pApp);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:{
            this->onAppInit_();
        }
            break;
        case APP_CMD_TERM_WINDOW:{
            this->onWindowTerminate_();
        }
            break;
        default:
            break;
    }
}

void AppStateManager::setUserData(void* _userData) {
    this->userData = _userData;
}

void AppStateManager::setCurrentAppContext(android_app *_current_app_context) {
    this->current_app_context = _current_app_context;
}

android_app *AppStateManager::getCurrentAppContext() {
    return this->current_app_context;
}
