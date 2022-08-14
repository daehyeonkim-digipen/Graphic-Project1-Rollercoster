#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Value Noise
//CS250
//Spring, 2022

uniform sampler2D depthmap;

in vec2 textureCoords;

out vec4 outColor;

void main(void)
{
    float color = texture2D(depthmap, textureCoords).r;
    outColor = vec4(vec3(color), 1.0);
}
