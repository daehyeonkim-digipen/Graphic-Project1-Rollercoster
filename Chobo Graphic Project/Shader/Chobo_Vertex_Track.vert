#version 330 core

layout (location = 0) in vec3 pos;

out vec4 vShadPos;

void main() {
    gl_Position = vec4(pos, 1.0f);
    vShadPos = vec4(pos, 1.0);
}