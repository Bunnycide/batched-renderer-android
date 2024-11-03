#version 320 es

precision mediump float;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;

void main(){
    gl_Position = a_position;
}