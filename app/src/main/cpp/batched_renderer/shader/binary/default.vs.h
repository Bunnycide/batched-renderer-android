#ifndef DEFAULT_VS_H
#define DEFAULT_VS_H

#include <string_view>

const std::string_view default_vs = R"(#version 320 es

precision mediump float;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_view;

void main(){
    gl_Position = vec4((u_model * u_view * u_proj * a_position).xy, 1.0f, 1.0f);
})";

#endif // DEFAULT_VS_H
