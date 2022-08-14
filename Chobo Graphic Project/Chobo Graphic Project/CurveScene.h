#pragma once
#include <map>
#include <vector>
#include "math.h"
#include "Scene.h"

enum class Curve {
	Hermite,
	Catmull,
};

class CurveScene : public Scene
{
private:
	struct Point {
		float x;
		float y;
		Vec2 tan;
		bool selected = false;
		bool tanselected = false;

		Point(float x, float y, Vec2 tan = { 0, 0 }) : x(x), y(y), tan(tan) {}
		void update();
	};
	std::map<int, Point*> hermitepoints;
	int hermitecount = 2;

	std::map<int, Point*> catmullpoints;
	int catmullcount = 4;

	Vec2 Hermite(Point* p1, Point* p2, float t);
	Vec2 Catmull(Point* p1, Point* p2, Point* p3, Point* p4, float t);

	struct Dot {
		float x;
		float y;
	};
	std::vector<Dot> Hdots;
	std::vector<Dot> Cdots;

	void drawHcurve();
	void drawCcurve();
	void updatedots();
	bool showvec = true;
	bool showpnt = true;
	bool looping = false;
	int linecount = 20;

	Curve curve = Curve::Hermite;

public:
	CurveScene() {
		hermitepoints[1] = new Point(300, 400, { 100, 0 });
		hermitepoints[2] = new Point(700, 400, { 0, 100 });

		catmullpoints[1] = new Point(300, 350);
		catmullpoints[2] = new Point(400, 450);
		catmullpoints[3] = new Point(600, 450);
		catmullpoints[4] = new Point(700, 350);
	}
	void update() override;
	void draw() override;
	bool select = false;
	bool tanselect = false;
};
