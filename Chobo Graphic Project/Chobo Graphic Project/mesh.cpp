#include <GL/glew.h>
#include <array>
#include <algorithm>
#include "mesh.h"

void SendVertexData(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);

    glGenBuffers(1, &mesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER,
        mesh.numVertices * vertexSize, &mesh.vertexBuffer[0],
        GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        mesh.numIndices * indexSize, &mesh.indexBuffer[0],
        GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type,
            vLayout[i].normalized, vertexSize, (void*)vLayout[i].offset);
    }
}

void BuildIndexBuffer(int stacks, int slices, Mesh& mesh) {
    int p0 = 0, p1 = 0, p2 = 0;
    int p3 = 0, p4 = 0, p5 = 0;
    const int stride = slices + 1;
    for (int i = 0; i < stacks; ++i) {
        int current_row = i * stride;
        for (int j = 0; j < slices; ++j) {
            p0 = current_row + j;
            p1 = p0 + 1;
            p2 = p1 + stride;
            if (!DegenerateTri(mesh.vertexBuffer[p0].pos, mesh.vertexBuffer[p1].pos, mesh.vertexBuffer[p2].pos)) {
                addIndex(mesh, p0);
                addIndex(mesh, p1);
                addIndex(mesh, p2);
            }
            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;
            if (!DegenerateTri(mesh.vertexBuffer[p3].pos, mesh.vertexBuffer[p4].pos, mesh.vertexBuffer[p5].pos)) {
                addIndex(mesh, p3);
                addIndex(mesh, p4);
                addIndex(mesh, p5);
            }
        }
    }
}

void addVertex(Mesh& mesh, const Vertex& v) {
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}

void addIndex(Mesh& mesh, int index) {
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;
    if (mesh.numIndices % 3 == 0) {
        ++mesh.numTris;
    }
}

Mesh CreatePlane(int stacks, int slices)
{
    Mesh mesh;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;

            v.pos = Vec3(col - 0.5f, 0.5f - row, 0.f);
            v.nrm = Vec3(0.0f, 0.0f, 1.0f);
            v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh CreateCube(int stacks, int slices) {
    Mesh planeMesh = CreatePlane(stacks, slices);
    Mesh mesh;
    Vec3 const translateArray[] = {
        Vec3(+0.0f, +0.0f, +0.5f),
        Vec3(+0.0f, +0.0f, -0.5f),
        Vec3(+0.5f, +0.0f, +0.0f),
        Vec3(-0.5f, +0.0f, +0.0f),
        Vec3(+0.0f, +0.5f, +0.0f),
        Vec3(+0.0f, -0.5f, +0.0f)
    };
    Vec2 const rotateArray[] = {
        Vec2(+0.0f, +0.0f),
        Vec2(+0.0f, (float)+PI),
        Vec2(+0.0f, (float)+HALF_PI),     
        Vec2(+0.0f, (float)-HALF_PI),
        Vec2((float)-HALF_PI, +0.0f),
        Vec2((float)+HALF_PI, +0.0f)
    };

    for (int i = 0; i < 6; ++i) {
        glm::mat4 transformMat = Translate(translateArray[i]) *
            Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *
            Rotate(rotateArray[i][XINDEX], { 1,0,0 });
        for (int j = 0; j < planeMesh.numVertices; ++j) {
            Vertex v;
            v.pos = -Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].pos, 1.0));
            v.nrm = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
            v.uv = planeMesh.vertexBuffer[j].uv;
            v.pos = RoundDecimal(v.pos);
            v.nrm = RoundDecimal(-v.nrm);
            addVertex(mesh, v);
        }
        for (int j = 0; j < planeMesh.numIndices; ++j) {
            addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
        }
    }
    return mesh;
}

Mesh CreateSphere(int stacks, int slices) {
    Mesh mesh;
    float radius = 0.5f;
    for (int i = 0; i <= stacks; ++i) {
        float row = (float)i / (float)stacks;
        float beta = PI * (row - 0.5f);
        for (int j = 0; j <= slices; ++j) {
            float  col = (float)j / (float)slices;
            float  alpha = col * TWO_PI;
            Vertex v;
            v.pos = Vec3(radius * sin(alpha) * cos(beta), radius * sin(beta), radius * cos(alpha) * cos(beta));
            v.nrm = Vec3(v.pos.x, v.pos.y, v.pos.z) / radius;
            v.uv = Vec2(1 - col, 1 - row);
            addVertex(mesh, v);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}

Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle) {
    float R = 0.35f;
    float r = 0.15f;
    float angle = endAngle - startAngle;
    Mesh mesh;
    for (int i = 0; i <= stacks; ++i) {
        float row = (float)i / (float)stacks;
        float alpha = row * angle + startAngle;
        float sinAlpha = sin(alpha);
        float cosAlpha = cos(alpha);
        for (int j = 0; j <= slices; ++j) {
            float  col = (float)j / (float)slices;
            float  beta = col * TWO_PI;
            Vertex v;
            v.pos = Vec3((R + r * cos(beta)) * sinAlpha, -r * sin(beta), (R + r * cos(beta)) * cosAlpha);
            Vec3 n = v.pos - Vec3(R * sin(alpha), 0.f, R * cos(alpha));
            v.nrm = n / r;
            v.uv = Vec2(1 - row, 1 - col);
            addVertex(mesh, v);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}

Mesh CreateCylinder(int stacks, int slices) {
    constexpr float radius = 0.5f;
    Mesh mesh;
    float row, col, alpha;
    //@todo: IMPLEMENT ME

    for (float i = 0; i <= stacks; i++) {
        row = i / stacks; // [0.0, 1.0]
        for (float j = 0; j <= slices; j++) {
            col = j / slices; // [0.0, 1.0]
            alpha = col * 2.0f * PI;
            Vertex vert;
            vert.pos.x = radius * sin(alpha);
            vert.pos.y = row - 0.5f;
            vert.pos.z = radius * cos(alpha);
            vert.nrm.x = vert.pos.x / radius;
            vert.nrm.y = 0.0;
            vert.nrm.z = vert.pos.z / radius;
            vert.uv = Vec2(col, row);
            addVertex(mesh, vert);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);

    for (float j = 0; j < slices; j++)
    {
        Vertex vert;

        col = j / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = 0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = 1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pi

        col = (j + 1.f) / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = 0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = 1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pj

        vert.pos.x = 0;
        vert.pos.y = 0.5;
        vert.pos.z = 0;
        vert.nrm.x = 0.0;
        vert.nrm.y = 1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//p0


    }
    for (double j = 0; j < slices; j++)
    {
        Vertex vert;

        col = static_cast<float>(j) / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = -0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pi

        vert.pos.x = 0;
        vert.pos.y = -0.5;
        vert.pos.z = 0;
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//p0

        col = (static_cast<float>(j) + 1.f) / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = -0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pj

    }

    return mesh;
}

Mesh CreateCone(int stacks, int slices) {
    constexpr float radius = 0.5f;
    Mesh mesh;
    float row, col, alpha;
    //@todo: IMPLEMENT ME

    for (float i = 0; i <= stacks; i++) {
        row = i / stacks; // [0.0, 1.0]
        for (float j = 0; j <= slices; j++) {
            col = j / slices; // [0.0, 1.0]
            alpha = col * 2.0f * PI;
            Vertex vert;
            float  h = row - 0.5f;
            vert.pos = Vec3(radius * (0.5f - h) * sin(alpha), h, radius * (0.5f - h) * cos(alpha));
            Vec3 vecnorm = Vec3(vert.pos.x / radius, radius, vert.pos.z / radius);
            float length = (float)sqrt(pow(vecnorm.x, 2) + pow(vecnorm.y, 2) + pow(vecnorm.z, 2));
            vert.nrm = vecnorm / length;
            vert.uv = Vec2(col, 1 - row);
            addVertex(mesh, vert);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);
    for (double j = 0; j < slices; j++)
    {
        Vertex vert;

        col = static_cast<float>(j) / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = -0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pi

        vert.pos.x = 0;
        vert.pos.y = -0.5;
        vert.pos.z = 0;
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//p0

        col = (static_cast<float>(j) + 1.f) / slices; // [0.0, 1.0]
        alpha = col * 2.0f * PI;
        vert.pos.x = radius * sin(alpha);
        vert.pos.y = -0.5;
        vert.pos.z = radius * cos(alpha);
        vert.nrm.x = 0.0;
        vert.nrm.y = -1.;
        vert.nrm.z = 0.0;
        vert.uv = Vec2(vert.pos.x, vert.pos.z);
        addVertex(mesh, vert);
        addIndex(mesh, mesh.numVertices - 1);//pj

    }
    return mesh;
}

Mesh CreateIcosahedron() {
    Mesh mesh;
    mesh.indexBuffer = { 2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 1, 5, 0, 11, 6, 7, 11, 7,
        8, 11, 8, 9, 11, 9, 10, 11, 10, 6, 1, 2, 6, 2, 3, 7, 3, 4, 8, 4,
        5, 9, 5, 1, 10, 2, 7, 6, 3, 8, 7, 4, 9, 8, 5, 10, 9, 1, 6, 10 };
    mesh.vertexBuffer = { Vertex(Vec3(0.000f, 0.000f, 1.000f) / Vec3(2)), Vertex(Vec3(0.894f, 0.000f, 0.447f) / Vec3(2)),
        Vertex(Vec3(0.276f, 0.851f, 0.447f) / Vec3(2)), Vertex(Vec3(-0.724f, 0.526f, 0.447f) / Vec3(2)),
        Vertex(Vec3(-0.724f, -0.526f, 0.447f) / Vec3(2)), Vertex(Vec3(0.276f, -0.851f, 0.447f) / Vec3(2)),
        Vertex(Vec3(0.724f, 0.526f, -0.447f) / Vec3(2)), Vertex(Vec3(-0.276f, 0.851f, -0.447f) / Vec3(2)),
        Vertex(Vec3(-0.894f, 0.000f, -0.447f) / Vec3(2)), Vertex(Vec3(-0.276f, -0.851f, -0.447f) / Vec3(2)),
        Vertex(Vec3(0.724f, -0.526f, -0.447f) / Vec3(2)), Vertex(Vec3(0.000f, 0.000f, -1.000f) / Vec3(2)),
    };
    for (int i = 0; i < mesh.vertexBuffer.size(); ++i) {
        mesh.vertexBuffer[i].uv = Vec2(abs(mesh.vertexBuffer[i].pos.x), abs(mesh.vertexBuffer[i].pos.y));
    }
    std::reverse(mesh.indexBuffer.begin(), mesh.indexBuffer.end());
    mesh.numIndices = static_cast<int>(mesh.indexBuffer.size());
    mesh.numVertices = static_cast<int>(mesh.vertexBuffer.size());
    return mesh;
}