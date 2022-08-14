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
uniform mat4 LmodeMat;

uniform vec4 color;
uniform int mesh;
uniform bool sea;

uniform sampler2D noise;

in vec4 teShadPos[];
in vec2 teUV[];
in int teGrass[];

out vec4 gShadPos;
out vec2 gUV;
out vec3 gNRM;
out vec4 gColor;
out float gDIST;

vec4 Verts[3];
vec4 Center;
vec3 Normal;
vec3 NNormal;

float rand(vec3 co) {
    return fract(sin(dot(co.xyz, vec3(12.9898, 78.233, 53.539))) * 43758.5453);
}

mat3 angleAxis(float angle, vec3 axis) {
    float s = sin(angle);
    float c = cos(angle);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat3(t * x * x + c, t * x * y - s * z, t * x * z + s * y,
        t * x * y + s * z, t * y * y + c, t * y * z - s * x,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c);
}

void main() {
    int normult = 1;
    if (mesh == 0) {
        normult = -1;
    }
    Verts[0] = gl_in[0].gl_Position;
    Verts[1] = gl_in[1].gl_Position;
    Verts[2] = gl_in[2].gl_Position;
    Center = (Verts[0] + Verts[1] + Verts[2]) / 3;
    vec3 A = Verts[2].xyz - Verts[0].xyz;
    vec3 B = Verts[1].xyz - Verts[0].xyz;
    Normal = normult * vec3(nrmMat * vec4(normalize(cross(A, B)), 0.0));
    gNRM = Normal;
    if (sea == false) {
        if (teGrass[0] == 1) {
            gColor = color;
        }
        else {
            gColor = vec4(0.75, 0.7, 0.5, 1);
        }

        gl_Position = mvpMat * rotMat * Verts[0];
        gDIST = length(gl_Position.xyz);
        gShadPos = teShadPos[0];
        EmitVertex();
        gl_Position = mvpMat * rotMat * Verts[1];
        gDIST = length(gl_Position.xyz);
        gShadPos = teShadPos[1];
        EmitVertex();
        gl_Position = mvpMat * rotMat * Verts[2];
        gDIST = length(gl_Position.xyz);
        gShadPos = teShadPos[2];
        EmitVertex();
        EndPrimitive();

        vec4 pos = mvpMat * rotMat * Center;
        vec3 NDCpos = pos.xyz / pos.w;
        if (teGrass[0] == 1 && NDCpos.x <= 1.5 && NDCpos.x >= -1.5 && NDCpos.y <= 1.5 && NDCpos.y >= -1.5 && NDCpos.z >= 0) {

            float press = float(texture2D(noise, teUV[0])) * 3;

            Normal = normalize(cross(A, B));
            B = normalize(angleAxis(rand(Verts[0].xyz) * 3.14f, Normal) * B);
            NNormal = normult * vec3(nrmMat * vec4(normalize(angleAxis(rand(Verts[1].xyz) * 3.14f * 1 * press / 10, B) * Normal), 0.0));

            gl_Position = mvpMat * rotMat * (Center);
            gShadPos = LmodeMat * rotMat * (Center);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.3, 0, 1);
            EmitVertex();
            gl_Position = mvpMat * rotMat * (Center + vec4(B, 0.0) / 30);
            gShadPos = LmodeMat * rotMat * (Center + vec4(B, 0.0) / 30);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.3, 0, 1);
            EmitVertex();

            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) / 240) + vec4(NNormal, 0.0) / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) / 240) + vec4(NNormal, 0.0) / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.6, 0, 1);
            EmitVertex();
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 7 / 240) + vec4(NNormal, 0.0) / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 7 / 240) + vec4(NNormal, 0.0) / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.6, 0, 1);
            EmitVertex();

            NNormal = normult * vec3(nrmMat * vec4(normalize(angleAxis(rand(Verts[1].xyz) * 3.14f * 1.3 * press / 10, B) * Normal), 0.0));
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 2 / 240) + vec4(NNormal, 0.0) * 2 / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 2 / 240) + vec4(NNormal, 0.0) * 2 / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.7, 0, 1);
            EmitVertex();
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 6 / 240) + vec4(NNormal, 0.0) * 2 / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 6 / 240) + vec4(NNormal, 0.0) * 2 / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.7, 0, 1);
            EmitVertex();

            NNormal = normult * vec3(nrmMat * vec4(normalize(angleAxis(rand(Verts[1].xyz) * 3.14f * 1.7 * press / 10, B) * Normal), 0.0));
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 3 / 240) + vec4(NNormal, 0.0) * 3 / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 3 / 240) + vec4(NNormal, 0.0) * 3 / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.8, 0, 1);
            EmitVertex();
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 5 / 240) + vec4(NNormal, 0.0) * 3 / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 5 / 240) + vec4(NNormal, 0.0) * 3 / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.8, 0, 1);
            EmitVertex();

            NNormal = normult * vec3(nrmMat * vec4(normalize(angleAxis(rand(Verts[1].xyz) * 3.14f * 2 * press / 10, B) * Normal), 0.0));
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 7 / 480) + vec4(NNormal, 0.0) * 7 / 40);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 7 / 480) + vec4(NNormal, 0.0) * 7 / 40);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.9, 0, 1);
            EmitVertex();
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) * 9 / 480) + vec4(NNormal, 0.0) * 7 / 40);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) * 9 / 480) + vec4(NNormal, 0.0) * 7 / 40);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 0.9, 0, 1);
            EmitVertex();

            NNormal = normult * vec3(nrmMat * vec4(normalize(angleAxis(rand(Verts[1].xyz) * 3.14f * 2.5 * press / 10, B) * Normal), 0.0));
            gl_Position = mvpMat * (rotMat * (Center + vec4(B, 0.0) / 60) + vec4(NNormal, 0.0) * 4 / 20);
            gShadPos = LmodeMat * (rotMat * (Center + vec4(B, 0.0) / 60) + vec4(NNormal, 0.0) * 4 / 20);
            gDIST = length(gl_Position.xyz);
            gColor = vec4(0, 1, 0, 1);
            EmitVertex();

            EndPrimitive();
        }
    }
    else {
        if (teGrass[0] == 0) {
            gColor = vec4(0, 0.4, 0.6, 0.8);
            Verts[0].z = 3.9 + float(texture2D(noise, vec2(1) - teUV[0])) / 3;
            Verts[1].z = 3.9 + float(texture2D(noise, vec2(1) - teUV[1])) / 3;
            Verts[2].z = 3.9 + float(texture2D(noise, vec2(1) - teUV[2])) / 3;
            gl_Position = mvpMat * rotMat * Verts[0];
            gShadPos = LmodeMat * rotMat * Verts[0];
            gDIST = length(gl_Position.xyz);
            EmitVertex();
            gl_Position = mvpMat * rotMat * Verts[1];
            gShadPos = LmodeMat * rotMat * Verts[1];
            gDIST = length(gl_Position.xyz);
            EmitVertex();
            gl_Position = mvpMat * rotMat * Verts[2];
            gShadPos = LmodeMat * rotMat * Verts[2];
            gDIST = length(gl_Position.xyz);
            EmitVertex();
            EndPrimitive();
        }
    }
}