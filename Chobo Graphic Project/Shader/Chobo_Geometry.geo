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
uniform bool islight;

in vec3 vNRM[];

out vec3 gNRM;
out float gDIST;

vec4 Verts[3];
vec3 Normal;

void EmitNewVertex(int i) {
	if (islight == false) {
		gl_Position = mvpMat * rotMat * vec4(Verts[i]);
	}
    else {
    	gl_Position = mvpMat * vec4(Verts[i]);
    }
    gNRM = vNRM[i];
    if (vNRM[i] == vec3(0)) {
    	gNRM = Normal;
    }
    gDIST = length(gl_Position.xyz);
    EmitVertex();
}

void main() {
	Verts[0] = gl_in[0].gl_Position;
	Verts[1] = gl_in[1].gl_Position;
	Verts[2] = gl_in[2].gl_Position;

	vec3 A = Verts[2].xyz - Verts[0].xyz;
    vec3 B = Verts[1].xyz - Verts[0].xyz;
    Normal = -vec3(nrmMat * vec4(normalize(cross(A,B)), 0.0));
	
	EmitNewVertex(0);
	EmitNewVertex(1);
	EmitNewVertex(2);
	EndPrimitive();
}