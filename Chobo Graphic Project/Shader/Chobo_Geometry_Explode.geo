#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(triangles) in;
layout(triangle_strip, max_vertices = 128) out;

uniform mat4 nrmMat;
uniform mat4 mvpMat;
uniform mat4 rotMat;

uniform float explode;
uniform int mesh;

out vec2 gUV;
out vec3 gNRM;
out float gDIST;

vec4 Verts[3];
vec3 Normal;

void main() {
    int normult = -1;
    if (mesh == 0) {
        normult = 1;
    }
	Verts[0] = gl_in[0].gl_Position;
	Verts[1] = gl_in[1].gl_Position;
	Verts[2] = gl_in[2].gl_Position;
	vec3 A = Verts[2].xyz - Verts[0].xyz;
    vec3 B = Verts[1].xyz - Verts[0].xyz;
    Normal = normult * vec3(nrmMat * vec4(normalize(cross(A,B)), 0.0));
	gNRM = Normal;

    gl_Position = mvpMat * (rotMat * Verts[0]);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[0] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[2]);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[2] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[1]);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[1] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex(); 
    gl_Position = mvpMat * (rotMat * Verts[0]);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[0] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();

    gl_Position = mvpMat * (rotMat * Verts[0] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[1] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();
    gl_Position = mvpMat * (rotMat * Verts[2] + vec4(Normal, 0.0) * explode);
    gDIST = length(gl_Position.xyz);
    EmitVertex();

	EndPrimitive();
}