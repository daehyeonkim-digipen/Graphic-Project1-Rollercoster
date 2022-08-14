#include "OpenGL.h"
#include "object.h"

void ShadowScene::setup() {
	SHADOW_WIDTH = 64 * pow(2, reso);
	SHADOW_HEIGHT = 64 * pow(2, reso);
	glDeleteTextures(1, &depthMap);
	depthMap = 0;
	glDeleteFramebuffers(1, &depthMapFBO);
	depthMapFBO = 0;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	GLenum depth_component = GL_DEPTH_COMPONENT32;
	if (bit == Compo24) {
		depth_component = GL_DEPTH_COMPONENT24;
	}
	else if (bit == Compo16) {
		depth_component = GL_DEPTH_COMPONENT16;
	}
	glTexStorage2D(GL_TEXTURE_2D, 1, depth_component, SHADOW_WIDTH, SHADOW_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat border[] = { 0 };
	if (lightup == Open) {
		border[0] = 1;
	}
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception{ "framebuffer not complete" };
	}
	if (objinit == false) {
		objectPush();
		objinit = true;
	}
}

void ShadowScene::objectPush() {
	auto light = new Object(part[LIGHT]);
	light->scale = { 1, 1, 1 };
	Objs.push_back(light);
	auto land = new Object(PLANE, 0, Vec4(1.f, 1.f, 1.f, 1.f), false);
	land->rotx = -PI / 2.;
	land->pos = { 0, -0.5, 0 };
	land->scale = { 20, 20, 20 };
	Objs.push_back(land);
	auto wall = new Object(PLANE, 0, Vec4(1.f, 1.f, 1.f, 1.f), false);
	wall->rotx = PI;
	wall->pos = { 0, 4.5, 10 };
	wall->scale = { 20, 10, 20 };
	Objs.push_back(wall);
	auto box = new Object(CUBE, 0, Vec4(1.f, 1.f, 1.f, 1.f), false);
	box->pos = { 4, 0, 2 };

	box->scale = { 0.5, 0.5, 0.5 };

	Objs.push_back(box);
	auto sph = new Object(SPHERE, 0, Vec4(0.5f, 1.f, 1.f, 1.f), false);
	sph->pos = { -4, 0, 2 };
	sph->scale = { 0.5, 0.5, 0.5 };
	Objs.push_back(sph);
	auto ico = new Object(ICOSAHEDRON, 0, Vec4(1.f, 0.5f, 1.f, 1.f), false);
	ico->pos = { 4, 0, -6 };
	ico->scale = { 0.5, 0.5, 0.5 };
	Objs.push_back(ico);
	auto tor = new Object(TORUS, 0, Vec4(1.f, 1.f, 0.5f, 1.f), false);
	tor->pos = { -4, 0, -6 };
	tor->scale = { 0.5, 0.5, 0.5 };
	Objs.push_back(tor);
	auto cyl = new Object(CYLINDER, 0, Vec4(1.f, 1.f, 1.f, 1.f), false);
	cyl->pos = { 0, 2, 4 };
	cyl->scale = { 2, 5, 2 };
	Objs.push_back(cyl);

	for (auto Obj : Objs) {
		Obj->whichShader = 8;
	}
}

void ShadowScene::update() {
	OpenGLCamera.ComputeViewProjMats();
	OpenGLLight.ComputeViewProjMats();
	OpenGLLight.ComputeLightMats();
	for (auto Obj : Objs) {
		Obj->ObjComputeMatrix();
	}
}

void ShadowScene::depthSetup() {
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(offsetFactor, offsetUnits);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for (auto Obj : Objs) {
		if (Obj->light == false) {
			Obj->ObjDraw(OpenGLLight.vpMat);
		}
	}
	glFlush();
}

void ShadowScene::draw() {
	depthSetup();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	for (auto Obj : Objs) {
		if(Obj->light) {
			Obj->ObjDraw(OpenGLLight.litMat);
		} 
		else {
			Obj->ObjDraw(lightMode ? OpenGLLight.vpMat : OpenGLCamera.vpMat);
		}
	}
	if (lightmap == true) {
		drawPlane();
	}
	if (frustum == true) {
		OpenGLLight.DrawFrustum();
	}
	ImGui::Begin("Shadow Modeling");
	ImGui::Checkbox("Light Mode", &lightMode);
	ImGui::Checkbox("Light Map", &lightmap);
	ImGui::Checkbox("Show Frustum", &frustum);
	ImGui::SliderFloat("Light Near Plane", &OpenGLLight.nearPlane, 1, 100);
	ImGui::SliderFloat("Light Far Plane", &OpenGLLight.farPlane, 10, 500);
	ImGui::SliderFloat("Field of View", &OpenGLLight.fov, SIXTEENTH_PI, HALF_PI + QUARTER_PI);
	ImGui::SliderFloat("Offset Factor", &offsetFactor, 0, 10);
	ImGui::SliderFloat("Offset Units", &offsetUnits, 0, 10);
	const char* lighttype[] = { "Border", "Open" };
	if (ImGui::Combo("Lighting Type", &lightup, lighttype, IM_ARRAYSIZE(lighttype))) {
		setup();
	}
	ImGui::Checkbox("Backlight", &backlight);
	const char* compo [] = { "32bit", "24bit", "16bit" };
	if (ImGui::Combo("Depth Component", &bit, compo, IM_ARRAYSIZE(compo))) {
		setup();
	}
	const char* resolution[] = { "64", "128", "256", "512", "1024", "2048", "4096", "8192" };
	if (ImGui::Combo("Shadow Resolution", &reso, resolution, IM_ARRAYSIZE(resolution))) {
		setup();
	}
	if (ImGui::Button("Reset", ImVec2(100, 50))) {
		OpenGLCamera.Reset();
		OpenGLLight.Reset();
		OpenGLLight.lightx = 0;
		OpenGLLight.lighty = 0;
		OpenGLLight.lightz = 0;
		lightmap = false;
		frustum = false;
		offsetFactor = 1;
		offsetUnits = 1;
		lightup = Border;
		bit = Compo32;
		reso = 6;
		lightMode = false;
		depth = false;
		backlight = false;
		setup();
	}
	ImGui::End();
}

void ShadowScene::drawPlane() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sScene.depthMap);

	GLuint pgm_handle = OpenGLShaderList.get_handle(9);
	glUseProgram(pgm_handle);

	GLuint textureLoc = glGetUniformLocation(pgm_handle, "depthmap");
	glUniform1i(textureLoc, GL_TEXTURE0);

	Mesh depthPlane = CreatePlane(1, 1);
	SendVertexData(depthPlane);
	glBindVertexArray(depthPlane.VAO);
	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, nullptr);
	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &depthPlane.VAO);
	glDeleteBuffers(1, &depthPlane.VBO);
	glDeleteBuffers(1, &depthPlane.IBO);
}