#ifndef DISPLAY_MANAGER_
#define DISPLAY_MANAGER_

#include "game-activity/native_app_glue/android_native_app_glue.h"
#include <vector>
#include <EGL/egl.h>

class DisplayManager{
private:
    // Singleton static singular instance
    static DisplayManager* displayManager;

    /**
     * @brief Construct a new Display Manager object
     *
     * @param pApp the app the commands are coming from
     */

    explicit DisplayManager(android_app* pApp);

    android_app* current_app_context;

    // Store EGL display and surface information
    EGLDisplay  egl_device_display_{EGL_NO_DISPLAY};
    EGLSurface  egl_surface_{EGL_NO_SURFACE};
    EGLContext  egl_context_{EGL_NO_CONTEXT};
    EGLConfig   egl_config_{nullptr};

    EGLint width_, height_{};

    /**
     * @brief Get the reference to the default display attached to the device
     *
     */
    void acquireDefaultDisplay();

    /**
     * @brief Picks a default display conf
     *
     */
    void chooseDisplayConf();

    /**
     * @brief Create a Window Surface object and stores it in egl_surface
     *
     */
    void createWindowSurface();

    /**
     * @brief Makes it so that the chosen display configuration is the one used by EGL
     *
     */
    void makeDisplayConfCurrent();


public:
    /**
     * @brief Delete the default constructor here
     *
     */
    DisplayManager() = delete;

    /**
     * @brief Make it so that the object can not be copied
     *
     */
    DisplayManager& operator=(DisplayManager*) = delete;

    /**
     * @brief initialises the display manager singleton with the current app context
     *
     */
    static void init(android_app*);

    /**
     * @brief swap the display data and the drawing surface data
     *
     */
    static void updateDisplay();
};

#endif // DISPLAY_MANAGER_