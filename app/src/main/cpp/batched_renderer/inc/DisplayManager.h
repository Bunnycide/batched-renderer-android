#ifndef __DISPLAY_MANAGER__
#define __DISPLAY_MANAGER__

#include "game-activity/native_app_glue/android_native_app_glue.h"
#include <vector>
#include <EGL/egl.h>

class IDrawFrameListener {
public:
    virtual void draw() = 0;
};

class DisplayManager{
private:
    static DisplayManager* displayManager;

    explicit DisplayManager(android_app* pApp);

    android_app* current_app_context;

    // EGL stuff
    EGLDisplay display_{};
    EGLSurface surface_{};
    EGLContext context_{};
    EGLConfig config_{};

    EGLint width_, height_{};

    IDrawFrameListener* drawFrameListener;

    std::vector<IDrawFrameListener*> drawFrameListeners;

    void acquireDefaultDisplay();
    void chooseDisplayConf();
    void createWindowSurface();
    void makeDisplayConfCurrent();


public:
    DisplayManager() = delete;
    static void init(android_app*);
    static void update(double* delta_time);

    DisplayManager& operator=(DisplayManager*) = delete;
    static void addDrawFrameListener(IDrawFrameListener *);
};

#endif // __DISPLAY_MANAGER__