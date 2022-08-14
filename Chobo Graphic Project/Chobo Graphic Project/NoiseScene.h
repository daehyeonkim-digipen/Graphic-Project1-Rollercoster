#pragma once
#include "math.h"
#include "Scene.h"

class NoiseScene : public Scene
{
public:
	void update() override;
	void draw() override;
private:
	Mat4 mat;

	int noisetype = Value;
	int prevnoise = Value;
	int smoothmethod = Linear;
	int prevmethod = Linear;

	float* noiseMap = new float[imageWidth * imageHeight]{ 0 };

	float frequency = 0.02f;
	float frequencyMult = 1.8f;
	float amplitudeMult = 0.35f;
	int numLayers = 5;
	bool terraceMode = false;
	int terrace = 5;
	bool mapping = false;

	bool isMoved = false;
	float moveTime = 0.f;

	float magnify = 1.f;
};

