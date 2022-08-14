#version 450 core

uniform vec4 uColor;

layout (location=0) out vec4 fFragClr;

void main () {
  fFragClr = vec4(uColor);
}