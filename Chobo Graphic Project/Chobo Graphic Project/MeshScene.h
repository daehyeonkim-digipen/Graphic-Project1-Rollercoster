#pragma once
#include "Scene.h"
#include <vector>

class Object;

class MeshScene : public Scene
{
private:
	int count = 1;
	bool wire = false;
	bool backcull = true;

	int gridsize = 5;
	bool grid = false;
	bool meshchanged = true;
	bool delay = true;

	int stack = 1;
	int slice = 1;
	float angle1 = 0;
	float angle2 = TWO_PI;
	float col[3] = { 1.f, 1.f, 1.f };

	float moveTime = 0;
	bool move = false;

	enum GeoShad {
		None,
		Shrink,
		Bezier,
		Explode,
		Tessellation,
		Grass,
	};
	int geoshad = GeoShad::None;

	std::vector<Object*> Objs;

public:
	float mindist = 5.f;
	float maxdist = 20.f;
	float fogcolor[3] = { 0.f, 0.f, 0.f };
	float shrink = 0.95f;
	int TessLevelInner = 1;
	int TessLevelOuter = 1;
	float explode = 0.f;
	bool sphere = false;
	float* noiseMap = new float[imageWidth * imageHeight]{ 0 };
	void update() override;
	void draw() override;
};
