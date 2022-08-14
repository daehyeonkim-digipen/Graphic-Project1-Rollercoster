//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022
#include "OpenGL.h"
#include <stdexcept>
#include "object.h"

int OpenGL::windowWidth;
int OpenGL::windowHeight;

void OpenGL::create_window(int w, int h, std::string t)
{
	windowWidth = w;
	windowHeight = h;
	if (!glfwInit()) {
		throw std::runtime_error("GLFW init has failed - abort program!!!");
	}
	glfwSetErrorCallback(nullptr);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window_ptr = glfwCreateWindow(windowWidth, windowHeight, t.c_str(), NULL, NULL);
	if (!window_ptr) {
		throw std::runtime_error("GLFW unable to create OpenGL context - abort program\n");
	}

	glfwMakeContextCurrent(window_ptr);

	GLenum err = glewInit();
	if (GLEW_OK != err || !GLEW_VERSION_3_3) {
		throw std::runtime_error("Driver doesn't support OpenGL 3.3 - abort program");
	}
	glClearColor(1.f, 1.f, 1.f, 1.f);
}

Vec2 OpenGL::window_size()
{
	return Vec2{ windowWidth, windowHeight };
}

OpenGL::OpenGL(const std::string& title)
{
	windowWidth = 1000;
	windowHeight = 800;
	//네모 하나, 네모 각 꼭지점과 대응하는 픽셀
	create_window(1000, 800, title);
	OpenGLInput = Input();
	OpenGLShaderList = ShaderList();
	OpenGLCamera = Camera(window_ptr, windowWidth, windowHeight);

	OpenGLLight = Light(window_ptr);
	OpenGLPolygon = Polygon(windowWidth, windowHeight);
	scene = &mScene;

	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex.vert", "../Shader/Chobo_Fragment.frag", "../Shader/Chobo_Geometry.geo");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Noise.vert", "../Shader/Chobo_Fragment_Noise.frag");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Polygon.vert", "../Shader/Chobo_Fragment_Polygon.frag");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Shrink.vert", "../Shader/Chobo_Fragment_Shrink.frag", "../Shader/Chobo_Geometry_Shrink.geo");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Bezier.vert", "../Shader/Chobo_Fragment_Bezier.frag", "../Shader/Chobo_Geometry_Bezier.geo");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Explode.vert", "../Shader/Chobo_Fragment_Explode.frag", "../Shader/Chobo_Geometry_Explode.geo");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Reso.vert", "../Shader/Chobo_Fragment_Reso.frag", "../Shader/Chobo_Geometry_Reso.geo",
		"../Shader/Chobo_Tess_Control_Reso.tesc", "../Shader/Chobo_Tess_Evaluation_Reso.tese");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Grass.vert", "../Shader/Chobo_Fragment_Grass.frag", "../Shader/Chobo_Geometry_Grass.geo",
		"../Shader/Chobo_Tess_Control_Grass.tesc", "../Shader/Chobo_Tess_Evaluation_Grass.tese");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Shadow.vert", "../Shader/Chobo_Fragment_Shadow.frag", "../Shader/Chobo_Geometry_Shadow.geo");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Map.vert", "../Shader/Chobo_Fragment_Map.frag");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_GrassFin.vert", "../Shader/Chobo_Fragment_GrassFin.frag", "../Shader/Chobo_Geometry_GrassFin.geo",
		"../Shader/Chobo_Tess_Control_GrassFin.tesc", "../Shader/Chobo_Tess_Evaluation_GrassFin.tese");
	OpenGLShaderList.add_new_handle("../Shader/Chobo_Vertex_Track.vert", "../Shader/Chobo_Fragment_Track.frag", "../Shader/Chobo_Geometry_Track.geo");
	OpenGLCamera.ComputeViewProjMats();
	OpenGLLight.ComputeViewProjMats();
	sScene.setup();
	fScene.setup();
	for (int i = 0; i < NUM_MESHES; ++i) {
		SendVertexData(meshes[i]);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void OpenGL::Update()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	OpenGLCamera.Update();
	OpenGLLight.Update();
	scene->update();
	OpenGLInput.update_prev_pos();
}

void OpenGL::draw()
{
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);

	glClearBufferfv(GL_COLOR, 0, bgColor);
	glClearBufferfv(GL_DEPTH, 0, &one);
	
	ImGui::Begin("Scene");
	if (ImGui::Button("Model", ImVec2(100, 50))) {
		scene = &mScene; 
		OpenGLCamera.Reset();
		OpenGLCamera.SetCamSpeed(0.15f);
		OpenGLLight.Reset();
		sScene.lightMode = false;
		OpenGLLight.shadow = false;
		MeshReset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Noise", ImVec2(100, 50))) {
		scene = &nScene;
		OpenGLCamera.Reset();
		OpenGLCamera.SetCamSpeed(0.3f);
		MeshReset();
	}
	if (ImGui::Button("Curve", ImVec2(100, 50))) {
		scene = &cScene;
		OpenGLCamera.Reset();
		MeshReset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Shadow", ImVec2(100, 50))) {
		scene = &sScene;
		OpenGLCamera.Reset();
		OpenGLCamera.SetCamSpeed(0.15f);
		OpenGLLight.Reset();
		OpenGLLight.lightx = 0;
		OpenGLLight.lighty = 0;
		OpenGLLight.lightz = 0;
		OpenGLLight.shadow = true;
		MeshReset();
	}
	if (ImGui::Button("Final", ImVec2(208, 50))) {
		scene = &fScene;
		OpenGLCamera.Reset();
		OpenGLCamera.cam = { 0, 0, 0 };
		OpenGLCamera.camspeed = 1.f;
		OpenGLLight.Reset();
		OpenGLLight.lightx = 0;
		OpenGLLight.lighty = 695;
		OpenGLLight.lightz = 0;
		OpenGLLight.lightFront = Vec3(0, -5, 0) - Vec3(OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz);
		OpenGLLight.fov = QUARTER_PI;
		OpenGLLight.nearPlane = 300.f;
		OpenGLLight.farPlane = 1000.f;
		sScene.lightMode = false;
		OpenGLLight.shadow = true;
		MeshReset();
		fScene.moveTime = 0;
	}
	scene->draw();
	ImGui::End();
	if (scene != &cScene) {
		ImGui::Begin("Camera");
		ImGui::Checkbox("Camera Follow Mouse", &OpenGLCamera.camMove);
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window_ptr);
	OpenGLPolygon.glsetting.clear();
}

void OpenGL::cleanup()
{
	glBindVertexArray(0);
	for (int i = 0; i < NUM_MESHES; ++i)
	{
		glDeleteVertexArrays(1, &meshes[i].VAO);
		glDeleteBuffers(1, &meshes[i].VBO);
		glDeleteBuffers(1, &meshes[i].IBO);
	}
}

bool OpenGL::is_end()
{
	return glfwWindowShouldClose(window_ptr);
}

void OpenGL::MeshReset() {
	Mesh tempmeshes[NUM_MESHES] = {
		CreatePlane(1, 1),
		CreateCube(1, 1),
		CreateSphere(32, 32),
		CreateCylinder(16, 32),
		CreateCone(16, 32),
		CreateTorus(16, 32, 0, TWO_PI),
		CreateIcosahedron(),
		CreateCone(16, 32),
		CreatePlane(imageWidth - 1, imageHeight - 1),
		CreatePlane(imageWidth - 1, imageHeight - 1)
	};
	for (int i = 0; i < NUM_MESHES; ++i) {
		meshes[i] = tempmeshes[i];
	}
}