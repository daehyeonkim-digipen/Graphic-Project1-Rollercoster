#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

layout(lines_adjacency) in;
layout(line_strip, max_vertices = 128) out;

uniform int uNumSections = 20;

void main() {
    float dt = 1.f / uNumSections;
    float t = 0;
    vec4 P0 = gl_in[0].gl_Position;
    vec4 P1 = gl_in[1].gl_Position;
    vec4 P2 = gl_in[2].gl_Position;
    vec4 P3 = gl_in[3].gl_Position;
    for (int i = 0; i <= uNumSections; ++i) {
        float one_minus_t = 1 - t;
        float one_minus_t2 = one_minus_t * one_minus_t;
        float one_minus_t3 = one_minus_t * one_minus_t2;
        float t2 = t * t;
        float t3 = t * t2;
        vec4 curve_point = one_minus_t3 * P0 +
        3 * t * one_minus_t2 * P1 +
        3 * t2 * one_minus_t * P2 +
        t3 * P3;
        gl_Position = curve_point;
        EmitVertex();
        t += dt;
    }
    EndPrimitive();
}