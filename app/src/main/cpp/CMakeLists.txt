# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("batched_renderer")

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(batched_renderer SHARED
        main.cpp
        AndroidOut.cpp
        batched_renderer/src/BatchedRenderer.cpp
        batched_renderer/src/DisplayManager.cpp
        batched_renderer/src/AppStateManager.cpp
        batched_renderer_app_state_manager.cpp
)

target_include_directories(batched_renderer PUBLIC batched_renderer/inc)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

# Configure libraries CMake uses to link your target library.
target_link_libraries(batched_renderer
        # The game activity
        game-activity::game-activity

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log)