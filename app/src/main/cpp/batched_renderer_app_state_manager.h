#include "AppStateManager.h"

class BatchedRendererApp : public AppStateManager {
private:
    // Control loop vars
    bool done = false;
    int timeout = 0;
    int events;
    android_poll_source *pSource;
    int inputPollResult;

    static BatchedRendererApp* batchedRendererApp;
    explicit BatchedRendererApp(android_app*);

    android_app* current_app_context{};

    void updateFrame_() override ;
    void onAppInit_() override ;
    void onWindowTerminate_() override ;

    double deltaTime;

public:
    BatchedRendererApp& operator=(BatchedRendererApp&) = delete;

    static void updateCmd(int32_t, android_app*);
    static void init(android_app*);
    static void updateFrame(android_app*);
};