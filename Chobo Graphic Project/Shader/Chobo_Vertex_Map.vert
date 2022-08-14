#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Value Noise
//CS250
//Spring, 2022

out vec2 textureCoords;

void main() {
    const vec2 positions[4] = vec2[](
        vec2(-0.5, -0.5),
        vec2(+0.5, -0.5),
        vec2(-0.5, +0.5),
        vec2(+0.5, +0.5)
    );
    const vec2 coords[4] = vec2[](
        vec2(0, 0),
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1)
    );

    textureCoords = coords[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}