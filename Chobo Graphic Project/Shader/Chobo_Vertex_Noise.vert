#version 330
precision mediump float;

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Value Noise
//CS250
//Spring, 2022

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

uniform mat4 mvpMat;
uniform mat4 rotMat;
out vec2 UV;
out vec3 NRM;

void main(void) 
{
    gl_Position = mvpMat * rotMat * vec4(pos, 1.0);
    NRM = nrm;
    UV = uv;
}