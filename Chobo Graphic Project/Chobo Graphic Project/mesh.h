#pragma once
#include <vector>
#include "math.h"

struct Vertex {
    Vertex(Vec3 pos, Vec3 nrm = Vec3(0), Vec3 uv = Vec3(0)) : pos(pos), nrm(nrm), uv(uv) {}
    Vertex() {}
    Vec3 pos, nrm;
    Vec2 uv;
};

const int vertexSize = sizeof(Vertex);
const int indexSize = sizeof(int);

struct VertexLayout
{
    int location;
    int size;
    int type;
    bool normalized;
    int offset;
};

const VertexLayout vLayout[] =
{
    { 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos) },
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) },
    { 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;

typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

struct Mesh {
    Mesh() : numVertices(0), numTris(0), numIndices(0) { }

    VertexBufferType vertexBuffer;
    IndexBufferType indexBuffer;

    int numVertices;
    int numTris;
    int numIndices;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint IBO = 0;
};

void SendVertexData(Mesh& mesh);
void BuildIndexBuffer(int stacks, int slices, Mesh& mesh);
void addVertex(Mesh& mesh, const Vertex& v);
void addIndex(Mesh& mesh, int index);

Mesh CreatePlane(int stacks, int slices);
Mesh CreateCube(int stacks, int slices);
Mesh CreateSphere(int stacks, int slices);
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle);
Mesh CreateCylinder(int stacks, int slices);
Mesh CreateCone(int stacks, int slices);
Mesh CreateIcosahedron();

enum MeshID {PLANE, CUBE, SPHERE, CYLINDER, CONE, TORUS, ICOSAHEDRON, LIGHTCONE, NOISEPLANE, EARTHPLA, NUM_MESHES };

inline Mesh meshes[NUM_MESHES] = {
    CreatePlane(1, 1),
    CreateCube(1, 1),
    CreateSphere(32, 32),
    CreateCylinder(16, 32),
    CreateCone(16, 32),
    CreateTorus(16, 32, 0, TWO_PI),
    CreateIcosahedron(),
    CreateCone(16, 32),
    CreatePlane(imageWidth - 1, imageHeight - 1),
    CreatePlane(imageWidth - 1, imageHeight - 1),
};

