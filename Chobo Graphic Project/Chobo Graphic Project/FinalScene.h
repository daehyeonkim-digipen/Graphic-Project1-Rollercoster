#pragma once
#include "Scene.h"
#include "object.h"

class FinalScene : public Scene
{ 
	struct Point {
		Vec3 tan;
		Vec3 up = { 0,1,0 };
		float angle;
		Object* obj;
		Point(Object* obj, Vec3 tan = { 0, 0, 0 }, float angle = 0) : obj(obj), tan(tan), angle(angle) {}
	};

	struct Dot
	{
		Vec3 pos;
		Point* point1;
		Point* point2;
		float rot;
	};

private:
	bool stop = false;
	bool depth = false;
	bool coaster = false;
	int SHADOW_WIDTH, SHADOW_HEIGHT;
	unsigned int depthMapFBO = 0;
	unsigned int windowWidth = 1000;
	unsigned int windowHeight = 800;

public:
	float* noiseMap = new float[imageWidth * imageHeight]{ 0 };
	float* grassMap = new float[imageWidth * imageHeight]{ 0 };
	unsigned int depthMap = 0;
	float moveTime = 0;
	int timeMult = 1;
	bool sea = false;
	bool lightunder = false;

	bool fog = false;
	bool toon = false;
	float mindist = 10.f;
	float maxdist = 300.f;

public:
	void setup();
	void update() override;
	void draw() override;

private:
	Vec3 Hermite(Point* p1, Point* p2, float t);
	float HermiteLength(Point* p1, Point* p2);

	void drawHcurve();
	void updatedots();
	void AddPoint(Vec3 pos, Vec3 tan = { 0, 0, 0 }, float angle = 0);

	void depthSetup();
private:
	int hermitecount = 0;
	std::map<int, Point*> hermitepoints;
	std::vector<Dot> Hdots;
	bool showpnt = false;
	bool showtrack = true;
	Point* selected = nullptr;
	std::vector<Object*> cars;
	std::vector<Object*> pillars;
	double timerSpeed;
	double carTimer = 0;
	unsigned carPos = 30;
	int lastPos;
	unsigned breaker;
	unsigned lift;
	bool camFixToCoaster = false;
	int pillarFactor = 200;
};

