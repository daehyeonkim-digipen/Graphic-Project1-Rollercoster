#version 460

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(vertices = 3) out;
 
in vec3 vPosition[];
in vec2 vUV[];

out vec3 tcPosition[];
out vec2 tcUV[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main() {
	gl_TessLevelInner[0] = TessLevelInner;

    gl_TessLevelOuter[0] = TessLevelOuter;
    gl_TessLevelOuter[1] = TessLevelOuter;
    gl_TessLevelOuter[2] = TessLevelOuter;

   	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcUV[gl_InvocationID] = vUV[gl_InvocationID];
}