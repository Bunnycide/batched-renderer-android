#include "DisplayManager/DisplayManager.h"
#include "../../AndroidOut.h"

DisplayManager* DisplayManager::displayManager = nullptr;

void DisplayManager::init(android_app* pApp) {
    if(displayManager == nullptr)
        displayManager = new DisplayManager(pApp);

    displayManager->acquireDefaultDisplay();
    displayManager->chooseDisplayConf();
    displayManager->createWindowSurface();
    displayManager->makeDisplayConfCurrent();
}

void DisplayManager::acquireDefaultDisplay() {
    aout << "Acquiring default display ..." << std::endl;

    egl_device_display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(egl_device_display_ == EGL_NO_DISPLAY){
        aout << "egl no display" << std::endl;
    }

    EGLint *major = nullptr, *minor = nullptr;

    if(! eglInitialize(egl_device_display_, major, minor)){
        aout << "Display initialisation failed" << std::endl;
    }
}

void DisplayManager::chooseDisplayConf() {
    aout << "Picking display config ..." << std::endl;

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
    eglChooseConfig(egl_device_display_, attribs, nullptr, 0, &numConfigs);

    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    eglChooseConfig(egl_device_display_, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

    egl_config_ = *std::find_if(
            supportedConfigs.get(),
            supportedConfigs.get() + numConfigs,
            [this](const EGLConfig &config) {
                EGLint red, green, blue, depth;
                if (eglGetConfigAttrib(egl_device_display_, config, EGL_RED_SIZE, &red)
                    && eglGetConfigAttrib(egl_device_display_, config, EGL_GREEN_SIZE, &green)
                    && eglGetConfigAttrib(egl_device_display_, config, EGL_BLUE_SIZE, &blue)
                    && eglGetConfigAttrib(egl_device_display_, config, EGL_DEPTH_SIZE, &depth)) {

                    aout << "Found config with " << red << ", " << green << ", " << blue << ", "
                         << depth << std::endl;
                    return red == 8 && green == 8 && blue == 8 && depth == 24;
                }
                return false;
            });

    aout << "Found " << numConfigs << " configs" << std::endl;
    aout << "Chose " << egl_config_ << std::endl;
}


void DisplayManager::createWindowSurface() {
    EGLint format;
    eglGetConfigAttrib(egl_device_display_, egl_config_, EGL_NATIVE_VISUAL_ID, &format);
    egl_surface_ = eglCreateWindowSurface(egl_device_display_, egl_config_, current_app_context->window, nullptr);
    if( egl_surface_ == EGL_NO_SURFACE) aout << "EGL no window surface " << std::endl;

    eglQuerySurface(egl_device_display_, egl_surface_, EGL_WIDTH, &width_);
    eglQuerySurface(egl_device_display_, egl_surface_, EGL_HEIGHT, &height_);

    aout << "The display surface is (" << width_ << "," << height_ << ")" << std::endl;
}

void DisplayManager::makeDisplayConfCurrent() {
    aout << "Making display current ..." << std::endl;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    egl_context_ = eglCreateContext(egl_device_display_, egl_config_, nullptr, contextAttribs);

    if(egl_context_ == EGL_NO_CONTEXT) aout << "No EGL context created " << std::endl;

    auto made_current = eglMakeCurrent(egl_device_display_, egl_surface_, egl_surface_, egl_context_);

    if(! made_current) aout << "ERROR: Could not make display current" << std::endl;
}

DisplayManager::DisplayManager(android_app *pApp) : width_{0}, height_{0}{
    this->current_app_context = pApp;
}

void DisplayManager::updateDisplay() {
    eglSwapBuffers(displayManager->egl_device_display_,
                                 displayManager->egl_surface_);
}

int DisplayManager::getWidth() {
    return displayManager->width_;
}

int DisplayManager::getHeight() {
    return displayManager->height_;
}
