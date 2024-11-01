#include <jni.h>

#include "AndroidOut.h"
#include "DisplayManager.h"
#include "batched_renderer_app_state_manager.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

void handle_cmd(android_app *pApp, int32_t cmd) {
    BatchedRendererApp::updateCmd(cmd, pApp);
}

bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app *pApp) {
    aout << "Welcome to android_main" << std::endl;
    BatchedRendererApp::init(pApp);
    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // Set input event filters (set it to NULL if the app wants to process all inputs).
    // Note that for key inputs, this example uses the default default_key_filter()
    // implemented in android_native_app_glue.c.
    android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    BatchedRendererApp::updateFrame(pApp);

//    // This sets up a typical game/event loop. It will run until the app is destroyed.
//    do {
//        // Process all pending events before running game logic.
//        bool done = false;
//        while (!done) {
//            // 0 is non-blocking.
//            int timeout = 0;
//            int events;
//            android_poll_source *pSource;
//            int result = ALooper_pollOnce(timeout, nullptr, &events,
//                                          reinterpret_cast<void**>(&pSource));
//            switch (result) {
//                case ALOOPER_POLL_TIMEOUT:
//                    [[clang::fallthrough]];
//                case ALOOPER_POLL_WAKE:
//                    // No events occurred before the timeout or explicit wake. Stop checking for events.
//                    done = true;
//                    break;
//                case ALOOPER_EVENT_ERROR:
//                    aout << "ALooper_pollOnce returned an error" << std::endl;
//                    break;
//                case ALOOPER_POLL_CALLBACK:
//                    break;
//                default:
//                    if (pSource) {
//                        pSource->process(pApp, pSource);
//                    }
//            }
//        }
//
//        // Check if any user data is associated. This is assigned in handle_cmd
//        if (pApp->userData) {
//            // We know that our user data is a Renderer, so reinterpret cast it. If you change your
//            // user data remember to change it here
//            // TODO: Replicate this in your setup
////            auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);
////
////            // Process game input
////            pRenderer->handleInput();
////
////            // Render a frame
////            pRenderer->render();
//        }
//    } while (!pApp->destroyRequested);
}
}
