#pragma once
#include "mesh.h"

struct Object {
	int meshID;
	int whichShader;
	Vec4 color;
	bool light;
	Mat4 rotMat = {}, nrmMat = {};

	int stack;
	int slice;
	float angle1;
	float angle2;
	Vec3 pos = {0, 0, 0};
	Vec3 scale = {1, 1, 1};
	bool toon = false;
	bool fog = false;

	bool final = false;

	int primitive = GL_TRIANGLES;

	void ObjComputeMatrix();
	void reset();
	void ObjDraw(const Mat4& MVPmat);
	void ObjNoiseDraw(const Mat4& MVPmat, float* noiseMap);
	void MouseObj();
	void UpdateMesh(int obj, int stack, int slice, float angle1 = -1, float angle2 = -1);

	float rotx = 0;
	float roty = 0;
	float rotz = 0;
	Object(int meshID, int whichShader, Vec4 color, bool light = false, int stack = 0, int slice = 0, float angle1 = 0, float angle2 = 0)
		: meshID(meshID), whichShader(whichShader), color(color), light(light), stack(stack), slice(slice), angle1(angle1), angle2(angle2) {
	}
};


enum PartID {
	PLA, CUB, SPH, CYL, CON, TOR, ICOSPH, LIGHT, NOISE, EARTH, SUN, MOON
};

inline Object part[12]{ 
	{
		PLANE, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 1, 1
	},
	{
		CUBE, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 1, 1
	},
	{
		SPHERE, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 32, 32
	}, 
	{
		CYLINDER, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 16, 32
	}, 
	{
		CONE, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 16, 32
	}, 
	{
		TORUS, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false, 16, 32, 0, TWO_PI
	},
	{
		ICOSAHEDRON, 0,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false
	},
	{
		LIGHTCONE, 0,
		Vec4(1.f, 1.f, 0.f, 1.f),
		true
	},
	{
		NOISEPLANE, 1,
		Vec4(1.f, 1.f, 1.f, 1.f),
		false
	},
	{
		EARTHPLA, 10,
		Vec4(0.0, 0.4, 0.0, 1.0),
		false
	},
	{
		SPHERE, 0,
		Vec4(2.0, 2.0, 2.0, 2.0),
		false
	},
	{
		SPHERE, 0,
		Vec4(3.0, 3.0, 3.0, 3.0),
		false
	},
};