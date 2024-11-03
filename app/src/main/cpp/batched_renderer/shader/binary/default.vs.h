#ifndef DEFAULT_VS_H
#define DEFAULT_VS_H

#include <string_view>

const std::string_view default_vs = R"(#version 320 es

precision mediump float;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

void main(){
    gl_Position = a_position;
})";

#endif // DEFAULT_VS_H
