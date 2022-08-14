#version 460

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(triangles, equal_spacing, cw) in;

uniform bool sphere;
uniform int mesh;

in vec4 tcShadPos[];
in vec3 tcPosition[];
in vec2 tcUV[];
in int tcGrass[];

out vec4 teShadPos;
out vec2 teUV;
out int teGrass;

void main() {
	float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    vec3 p0 = tcPosition[0] * u;
    vec3 p1 = tcPosition[1] * v;
    vec3 p2 = tcPosition[2] * w;

    vec3 point_on_triangle = p0 + p1 + p2;
    gl_Position = vec4(point_on_triangle, 1);

    teShadPos = tcShadPos[0] * u + tcShadPos[1] * v + tcShadPos[2] * w;
    teUV = tcUV[0] * u + tcUV[1] * v + tcUV[2] * w;
    if (tcGrass[0] == 1 || tcGrass[1] == 1 || tcGrass[2] == 1) {
        teGrass = 1;
    }
    else {
        teGrass = 0;
    }
}