#include <assert.h>
#include "DisplayManager.h"
#include "../../AndroidOut.h"

DisplayManager* DisplayManager::displayManager = nullptr;

void DisplayManager::update(double *delta_time) {

    if(! displayManager->drawFrameListeners.empty()){
        for(auto& listener  : displayManager->drawFrameListeners){
            listener->draw();
        }
    }
}

void DisplayManager::init(android_app* pApp) {
    if(displayManager == nullptr)
        displayManager = new DisplayManager(pApp);

    displayManager->acquireDefaultDisplay();
    displayManager->chooseDisplayConf();
    displayManager->createWindowSurface();
    displayManager->makeDisplayConfCurrent();


}

void DisplayManager::acquireDefaultDisplay() {
    aout << "Acquiring default display" << std::endl;

    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint *major = nullptr, *minor = nullptr;
    eglInitialize(display_, major, minor);

}

void DisplayManager::chooseDisplayConf() {
    aout << "Chose display config" << std::endl;

    constexpr EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };

    EGLint numConfigs;
    eglChooseConfig(display_, attribs, nullptr, 0, &numConfigs);

    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    eglChooseConfig(display_, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    auto tempDisplay = display_;
    config_ = *std::find_if(
            supportedConfigs.get(),
            supportedConfigs.get() + numConfigs,
            [this](const EGLConfig &config) {
                EGLint red, green, blue, depth;
                if (eglGetConfigAttrib(display_, config, EGL_RED_SIZE, &red)
                    && eglGetConfigAttrib(display_, config, EGL_GREEN_SIZE, &green)
                    && eglGetConfigAttrib(display_, config, EGL_BLUE_SIZE, &blue)
                    && eglGetConfigAttrib(display_, config, EGL_DEPTH_SIZE, &depth)) {

                    aout << "Found config with " << red << ", " << green << ", " << blue << ", "
                         << depth << std::endl;
                    return red == 8 && green == 8 && blue == 8 && depth == 24;
                }
                return false;
            });

    aout << "Found " << numConfigs << " configs" << std::endl;
    aout << "Chose " << config_ << std::endl;
}


void DisplayManager::createWindowSurface() {
    EGLint format;
    eglGetConfigAttrib(display_, config_, EGL_NATIVE_VISUAL_ID, &format);
    surface_ = eglCreateWindowSurface(display_, config_, current_app_context->window, nullptr);

}

void DisplayManager::makeDisplayConfCurrent() {
    aout << "Made the display context current" << std::endl;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    context_ = eglCreateContext(display_, config_, nullptr, contextAttribs);

    auto made_current = eglMakeCurrent(display_, surface_, surface_, context_);
    assert(made_current);
}

DisplayManager::DisplayManager(android_app *pApp) {
    this->current_app_context = pApp;
}

void DisplayManager::addDrawFrameListener(IDrawFrameListener *iDrawFrameListener) {
    if(iDrawFrameListener != nullptr){
        DisplayManager::displayManager->drawFrameListeners.push_back(iDrawFrameListener);
    }
}

