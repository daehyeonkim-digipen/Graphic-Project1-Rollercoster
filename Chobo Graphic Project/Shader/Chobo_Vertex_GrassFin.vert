#version 330
precision mediump float;

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;

uniform mat4 mvpMat;
uniform mat4 rotMat;
uniform mat4 nrmMat;
uniform mat4 LmodeMat;

out vec4 vShadPos;
out vec3 vPosition;
out vec2 vUV;
out int vGrass;

void main(void) 
{
    vGrass = 0;
    if (pos.z < 0.08) {
        vGrass = 1;
    }
    gl_Position = vec4(pos * vec3(50), 1.0);
    vec3 vpos = vec3(mvpMat * rotMat * vec4(pos * vec3(50), 1.0));
    vPosition = pos * vec3(50);
    vShadPos = LmodeMat * rotMat * vec4(pos * vec3(50), 1.0);
    vUV = uv;
}