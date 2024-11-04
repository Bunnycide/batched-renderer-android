#ifndef DEFAULT_FS_H
#define DEFAULT_FS_H

#include <string_view>

const std::string_view default_fs = R"(#version 320 es

precision mediump float;

layout (location = 0) out vec4 o_color;

void main(){
    o_color = vec4(0.6, 0.0, 0.0, 1.0);
})";

#endif // DEFAULT_FS_H
