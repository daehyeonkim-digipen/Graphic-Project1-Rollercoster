#version 460

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(lines) in;
layout(triangle_strip, max_vertices = 50) out;

uniform mat4 mvpMat;
uniform mat4 LmodeMat;
uniform vec3 front;
uniform vec3 final;
uniform vec3 left;

in vec4 vShadPos[];

out vec4 gShadPos;
out vec3 gNRM;
out float gDIST;

vec4 Verts[3];
vec3 Normal;

void main() {
	Verts[0] = gl_in[0].gl_Position - vec4(normalize(front) * 0.05, 0);
	Verts[1] = gl_in[1].gl_Position + vec4(normalize(front) * 0.05, 0);

	vec4 first = Verts[0];
	vec4 second = (Verts[0] - vec4(final / 3, 0) + vec4(left, 0));
	vec4 third = Verts[1];

	vec3 A = (third - first).xyz;
    vec3 B = (second - first).xyz;
    gNRM = -normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	EndPrimitive();

	first = (Verts[0] - vec4(final / 3, 0) + vec4(left, 0));
	second = Verts[1];
	third = (Verts[1] - vec4(final / 3, 0) + vec4(left, 0));

	A = (third - first).xyz;
    B = (second - first).xyz;
    gNRM = normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	EndPrimitive();

	first = Verts[0];
	second = (Verts[0] - vec4(final / 3, 0) - vec4(left, 0));
	third = Verts[1];

	A = (third - first).xyz;
    B = (second - first).xyz;
    gNRM = normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	EndPrimitive();

	first = (Verts[0] - vec4(final / 3, 0) - vec4(left, 0));
	second = (Verts[1] - vec4(final / 3, 0) - vec4(left, 0));
	third = Verts[1];

	A = (third - first).xyz;
    B = (second - first).xyz;
    gNRM = normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	EndPrimitive();

	first = (Verts[0] - vec4(final / 3, 0) + vec4(left, 0));
	second = (Verts[0] - vec4(final / 3, 0) - vec4(left, 0));
	third = (Verts[1] - vec4(final / 3, 0) + vec4(left, 0));

	A = (third - first).xyz;
    B = (second - first).xyz;
    gNRM = -normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();
	
	EndPrimitive();

	first = (Verts[0] - vec4(final / 3, 0) - vec4(left, 0));
	second = (Verts[1] - vec4(final / 3, 0) + vec4(left, 0));
	third = (Verts[1] - vec4(final / 3, 0) - vec4(left, 0));

	A = (third - first).xyz;
    B = (second - first).xyz;
    gNRM = normalize(cross(A,B));

	gl_Position = mvpMat * first;
	gShadPos = LmodeMat * first;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * second;
	gShadPos = LmodeMat * second;
	gDIST = length(gl_Position.xyz);
	EmitVertex();

	gl_Position = mvpMat * third;
	gShadPos = LmodeMat * third;
	gDIST = length(gl_Position.xyz);
	EmitVertex();
	
	EndPrimitive();
}