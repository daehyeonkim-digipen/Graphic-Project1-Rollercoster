#version 460
precision mediump float;

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

out vec3 vPosition;

void main(void) 
{
    gl_Position = vec4(pos, 1.0);
    vPosition = pos;
}