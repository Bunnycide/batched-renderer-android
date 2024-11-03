#include <jni.h>

#include "AndroidOut.h"
#include "DisplayManager/DisplayManager.h"
#include "AppStateManager/AppStateManager.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

#include "batched_renderer/BatchedRenderer.h"

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

void handle_cmd(android_app *pApp, int32_t cmd) {
    BatchedRenderer::handle_app_state(cmd);
}

bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app *pApp) {
    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    BatchedRenderer::init(pApp);

    BatchedRenderer::main_loop(pApp);

    android_app_set_motion_event_filter(pApp, motion_event_filter_func);
}
}
