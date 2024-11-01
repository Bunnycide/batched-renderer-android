#ifndef APP_STATE_MANAGER_
#define APP_STATE_MANAGER_

#include <cstdint>
#include <game-activity/native_app_glue/android_native_app_glue.h>

class AppStateManager {
private:
    void* userData{};

    void loop();

    android_app* current_app_context{};
public:
    void setUserData(void* );
    void setCurrentAppContext(android_app*);
    android_app* getCurrentAppContext();

    void updateCmd(int32_t, android_app* pApp);

    virtual // Called when the app is initialised
    void onAppInit_();

    virtual // Called when the frame is updated
    void updateFrame_();

    virtual // Called when window is about to be terminated
    void onWindowTerminate_();

};

#endif //APP_STATE_MANAGER_