#pragma once
#include "math.h"
class GLFWwindow;

class Light
{
public:
	float nearPlane;
	float farPlane;
	float topPlane;
	float bottomPlane;
	float aspect;
	float rightPlane;
	float leftPlane;
	float fov;

	int lightvecmult = 15;

	float yaw = 0;
	float pitch = 0;
	float lightspeed = 0.15f;

	float lightx;
	float lighty;
	float lightz;
	Vec3 lightFront;
	Vec3 lightUp;
	Mat4 viewMat, projMat, magMat, vpMat, litMat, ltnmMat, plaMat;
	GLFWwindow* window_ptr;
	float lastX;
	float lastY;
	bool firstMouse = true;
	bool shadow = false;

	std::vector<Vec4> points;
	int shadpgm;
	void ShadInit();

public:
	Light(GLFWwindow*);
	Light() = default;
	void ComputeViewProjMats();
	void ComputeLightMats();
	void Reset();
	void Update();
	void SetLightVec(int mult);
	void mouselight(double, double);
	void DrawFrustum();
};

