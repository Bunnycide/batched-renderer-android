# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("batched_renderer")

set(BR_DIR ${CMAKE_SOURCE_DIR}/batched_renderer)

execute_process(COMMAND "bash batched_renderer/shader/compile_shaders.sh")

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(batched_renderer SHARED
        main.cpp
        AndroidOut.cpp
        ${BR_DIR}/src/AppStateManager/AppStateManager.cpp
        ${BR_DIR}/src/DisplayManager/DisplayManager.cpp
        ${BR_DIR}/src/Shader/Shader.cpp
        ${BR_DIR}/src/Batcher/Batcher.cpp
        ${BR_DIR}/src/ClusterGenerator/cluster-generator.cpp
        ${BR_DIR}/src/Camera/Camera.cpp
        ${BR_DIR}/BatchedRenderer.cpp
)

target_include_directories(batched_renderer PUBLIC ${BR_DIR}/inc batched_renderer/third-party/glm)

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