#ifndef APP_STATE_MANAGER_
#define APP_STATE_MANAGER_

#include <game-activity/native_app_glue/android_native_app_glue.h>

/**
 * @brief manage the native activity state processing
 * */

class AppStateManager{
private:
    // The singleton
    static AppStateManager* appStateManagerInstance;

    // The android app context
    android_app* android_app_context = nullptr;

    // Evenet handling related vars
    int timeout = 0; // Polling timeout
    int events{0}; // Event read
    android_poll_source *pSource{};

    int result{0}; // After event poll result
    bool pollingDone = false; // Checks if the polling should stop if no events were reported back


protected:
    /**
     * @brief The draw calls to be implemented by the deriving class
     *
     */
    virtual void draw() = 0;

    // App States
    virtual void onWindowInit();
    virtual void onWindowTerminate();

public:
    void setAppContext(android_app* pApp);
    android_app* getAppContext();

    // Delete the default constructor and the assignment operatoe overload
    AppStateManager() = default;
    AppStateManager& operator=(AppStateManager&) = delete;

    // Only available constructor
    explicit AppStateManager(android_app* pApp_);

    // App state dispatch and process handler
    void handleAppState(int32_t);

    // App main loop
    void loop();
};

#endif // APP_STATE_MANAGER_
