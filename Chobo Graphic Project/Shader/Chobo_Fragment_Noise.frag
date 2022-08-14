#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Value Noise
//CS250
//Spring, 2022

uniform vec4 color;
uniform sampler2D noise;

in vec2 UV;
in vec3 NRM;

out vec4 outColor;

void main(void)
{
    float color = float(texture2D(noise, UV));
    outColor = vec4(vec3(color), 1.0);
}
