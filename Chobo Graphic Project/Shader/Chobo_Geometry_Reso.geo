#version 460

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 mvpMat;
uniform mat4 rotMat;
uniform mat4 nrmMat;

uniform float uShrink;
uniform int mesh;

out vec3 gNRM;
out float gDIST;

vec3 Verts[3];
vec3 Center;
vec3 Normal;

void EmitShrunkVertex(int i) {
	vec3 move_direction = Verts[i] - Center;
    gl_Position = mvpMat * rotMat * vec4(Center + uShrink * move_direction, 1.0);
    gNRM = Normal;
    gDIST = length(gl_Position.xyz);
    EmitVertex();
}

void main() {
int normult = 1;
    if (mesh == 0) {
        normult = -1;
    }
	Verts[0] = gl_in[0].gl_Position.xyz;
	Verts[1] = gl_in[1].gl_Position.xyz;
	Verts[2] = gl_in[2].gl_Position.xyz;

	Center = (Verts[0] + Verts[1] + Verts[2]) / 3;
	vec3 A = Verts[2] - Verts[0];
    vec3 B = Verts[1] - Verts[0];
    Normal = normult * vec3(nrmMat * vec4(normalize(cross(A,B)), 0.0));
	
	EmitShrunkVertex(0);
	EmitShrunkVertex(1);
	EmitShrunkVertex(2);
	EndPrimitive();
}