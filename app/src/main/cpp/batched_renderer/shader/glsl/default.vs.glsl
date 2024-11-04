#version 320 es

precision mediump float;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

uniform mat4 u_proj;
uniform mat4 u_model;
uniform mat4 u_view;

void main(){
    gl_Position = u_proj * u_model * u_view * a_position;
}