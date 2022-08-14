#version 460

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(vertices = 3) out;

in vec4 vShadPos[];
in vec3 vPosition[];
in vec2 vUV[];
in int vGrass[];

out vec4 tcShadPos[];
out vec3 tcPosition[];
out vec2 tcUV[];
out int tcGrass[];

float TessLevelInner = 2;
float TessLevelOuter = 2;

void main() {
	gl_TessLevelInner[0] = TessLevelInner;

    gl_TessLevelOuter[0] = TessLevelOuter;
    gl_TessLevelOuter[1] = TessLevelOuter;
    gl_TessLevelOuter[2] = TessLevelOuter;

    tcShadPos[gl_InvocationID] = vShadPos[gl_InvocationID];
   	tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcUV[gl_InvocationID] = vUV[gl_InvocationID];
    tcGrass[gl_InvocationID] = vGrass[gl_InvocationID];
}