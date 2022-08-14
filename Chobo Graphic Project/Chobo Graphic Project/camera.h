#pragma once
#include "math.h"

struct Object;

class Camera
{
public:
	float nearPlane;
	float farPlane;
	float topPlane;
	float bottomPlane;
	float aspect;
	float rightPlane;
	float leftPlane;

	float yaw = 270;
	float pitch = 0;
	float roll = 0;
	float camspeed = 0.15f;
	Vec3 cam;
	Vec3 camFront;
	Vec3 camUp;
	Mat4 viewMat, projMat, magMat, vpMat;
	GLFWwindow* window_ptr;
	float lastX;
	float lastY;
	bool firstMouse = true;
	bool camMove;

public:
	Camera(GLFWwindow*, int, int);
	Camera() = default;
	void ComputeViewProjMats();
	void Reset();
	void Update();
	void SetCamSpeed(float speed);
	void SetPosition(Vec3 pos);
	void mouseCam(double, double);
};

