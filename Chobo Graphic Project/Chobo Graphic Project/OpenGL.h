//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022
#pragma once

#include <string>
#include "ShaderList.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "camera.h"
#include "input.h"
#include "polygon.h"
#include "MeshScene.h"
#include "NoiseScene.h"
#include "CurveScene.h"
#include "light.h"
#include "ShadowScene.h"
#include "FinalScene.h"

inline Camera OpenGLCamera;
inline Light OpenGLLight;
inline ShaderList OpenGLShaderList;
inline Input OpenGLInput;
inline Polygon OpenGLPolygon;
inline MeshScene mScene;
inline NoiseScene nScene;
inline CurveScene cScene;
inline ShadowScene sScene;
inline FinalScene fScene;

class OpenGL
{
private:
	GLFWwindow* window_ptr;

private:
	static int windowWidth;
	static int windowHeight;

	Scene* scene;

	void MeshReset();

public:
	//ok
	OpenGL(const std::string& title);
	void create_window(int w, int h, std::string t);
	static Vec2 window_size();
	void Update();
	GLFWwindow* GetWindow() {
		return window_ptr;
	}
	void draw();
	bool is_end();
	void cleanup();
};

const GLfloat bgColor[] = { 0.f, 0.f, 0.f, 1.0f };
const GLfloat one = 1.0f;
