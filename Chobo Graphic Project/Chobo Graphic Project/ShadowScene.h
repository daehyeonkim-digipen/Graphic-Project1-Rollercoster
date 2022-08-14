#pragma once
#include "Scene.h"
#include <vector>

class Object;

class ShadowScene : public Scene
{
private:
	bool objinit = false;
	bool lightmap = false;
	bool frustum = false;
	std::vector<Object*> Objs;

	int SHADOW_WIDTH, SHADOW_HEIGHT;
	unsigned int depthMapFBO = 0;
	unsigned int windowWidth = 1000;
	unsigned int windowHeight = 800;

	float offsetFactor = 1;
	float offsetUnits = 1;

	int lightup = Border;
	int bit = Compo32;
	int reso = 6;

	void objectPush();
	void drawPlane();

public:
	bool lightMode = false;
	bool depth = false;
	bool backlight = false;
	unsigned int depthMap = 0;

public:
	void setup();
	void update() override;
	void draw() override;
	void depthSetup();
};

