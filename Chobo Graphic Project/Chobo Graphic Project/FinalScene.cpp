#include "OpenGL.h"
#include "noise.h"

void FinalScene::setup() {
	SHADOW_WIDTH = 32768;
	SHADOW_HEIGHT = 32768;
	glDeleteTextures(1, &depthMap);
	depthMap = 0;
	glDeleteFramebuffers(1, &depthMapFBO);
	depthMapFBO = 0;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	GLenum depth_component = GL_DEPTH_COMPONENT32;
	glTexStorage2D(GL_TEXTURE_2D, 1, depth_component, SHADOW_WIDTH, SHADOW_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	GLfloat border[] = { 1 };
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

	part[9].primitive = GL_PATCHES;
	part[9].rotx = PI / 2;
	part[9].final = true;
	part[10].scale = { 7, 7, 7 };
	part[10].final = true;
	part[11].scale = { 7, 7, 7 };
	part[11].final = true;
	
	AddPoint({ -7, -1, -6 }, {0, 0, 10}, 0);
	AddPoint({ -7, -1, 3 }, { 0, 0, 5 }, 0);
	AddPoint({ -4, 0, 5 }, { 5, 3, 0 }, 0);
	AddPoint({ 20, 21, 5 }, { 5, 0, 0 }, 0);
	AddPoint({ 24, 21, 9 }, { 0, 0, 5 }, 0);
	AddPoint({ 20, 21, 13 }, { -10, 0, 0 }, 0);
	AddPoint({ 6, -1, 13 }, { -30, 0, 0 }, 0);
	AddPoint({ -7, 8, 13 }, { 0, 10, 0 }, 0);
	AddPoint({ -1, 15, 12.5 }, { 10, 0, -1 }, 0);
	AddPoint({ 5, 8, 12 }, { 0, -10, 0 }, 0);
	AddPoint({ -8, -1, 12 }, { -30, 0, 0 }, 0.3);
	AddPoint({ -14, 13, 0 }, { 0, 0, -20 }, 0);
	AddPoint({ -14, 1, -12 }, { 0, 0, -20 }, 0);
	AddPoint({ -8, 11, -20 }, { 20, 0, 0 }, 0);
	AddPoint({ 2, -1, -20 }, { 15, 0, 0 }, 0);
	AddPoint({ 8, 5, -20 }, { 0, 4, 0 }, 0);
	AddPoint({ 4, 9, -20 }, { -10, 0, 0 }, 0);
	AddPoint({ 0, 5, -16 }, { 0, 0, 10 }, 2.5);
	AddPoint({ 4, 9, -12 }, { 10, 0, 0 }, 0);
	AddPoint({ 8, 5, -12 }, { 0, -4, 0 }, 0);
	AddPoint({ 2, -1, -12 }, { -15, 0, 0 }, 0);
	AddPoint({ -11, 6, -5 }, { 0, 0, 20 }, -2);
	AddPoint({ -1, -1, 3 }, { 15, 0, 0 }, 0);
	AddPoint({ 8, 6, 3 }, { 12, 0, 0 }, -0.3);
	AddPoint({ 14, 5.5, -4 }, { 0, -0.5, -12 }, -0.8);
	AddPoint({ 8, 5, -10 }, { -12, -0.5, 0 }, -1);
	AddPoint({ 2, 4.5, -4 }, { 0, -0.5, 12 }, -1);
	AddPoint({ 8, 4, 2 }, { 12, -0.5, 2 }, -1);
	AddPoint({ 14, 3.5, -4 }, { 0, -0.5, -12 }, -1);
	AddPoint({ 8, 3, -10 }, { -12, -0.5, 0 }, -1);
	AddPoint({ 2, 2.5, -4 }, { 0, -0.5, 12 }, -1);
	AddPoint({ 8, 2, 2 }, { 12, -0.5, 2 }, -1);
	AddPoint({ 16, 1.5, -6 }, { 0, -0.5, -15 }, -0.8);
	AddPoint({ 8, 1, -15 }, { -12, -0.5, 0 }, 0);
	AddPoint({ -2, 0, -15 }, { -12, 0, 0 }, 0);

	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));
	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));
	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));
	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));
	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));
	cars.push_back(new Object(SPHERE, 0, Vec4(.2f, .5f, 1.f, 1.f), false));

	updatedots();
	cars[0]->pos = Hdots[carPos].pos;
	timerSpeed = 0.5;

	for(int i = 0; i < Hdots.size() / pillarFactor + 10; i++)
	{ 
		pillars.push_back(new Object(
			CUBE, 0,
			Vec4(4.f, 4.f, 4.f, 1.f),
			false, 1, 1
		));
	}
	
}

void FinalScene::update() {
	if (OpenGLInput.IsRightMouseReleased() == true) {
		if (OpenGLCamera.camspeed == 1.f) {
			OpenGLCamera.SetCamSpeed(5.f);
		}
		else {
			OpenGLCamera.SetCamSpeed(1.f);
		}
	}

	OpenGLCamera.ComputeViewProjMats();
	OpenGLLight.ComputeViewProjMats();
	OpenGLLight.ComputeLightMats();
	  
	ValueNoise noise;
	noise.SetSmooth(SmoothMethod::SmoothStep);
	PerlinNoise Pnoise;
	Pnoise.SetSmooth(SmoothMethod::Quintic);
	Vec3 derivs = { 0, 0, 0 };
	for (unsigned j = 0; j < imageHeight; ++j) {
		for (unsigned i = 0; i < imageWidth; ++i) {
			Vec3 pNoise = Vec3(i, 0, j) * 0.01f + Vec3(moveTime / 2, 0, 0);
			float g = Pnoise.eval(pNoise, derivs);
			grassMap[j * imageWidth + i] = (g + 1) * 0.5f;
		}
	}
	for (unsigned j = 0; j < imageHeight; ++j) {
		for (unsigned i = 0; i < imageWidth; ++i) {
			Vec2 pNoise = Vec2(i, j) * 0.015f;
			noiseMap[j * imageWidth + i] = noise.eval(pNoise);
		}
	}
	if (stop == false) {
		moveTime += ImGui::GetIO().DeltaTime;
	}

	for (int i = 0; i < meshes[part[9].meshID].numVertices; ++i) {
		meshes[part[9].meshID].vertexBuffer[i].pos.z = noiseMap[i] / 5.f;
	}

	part[9].ObjComputeMatrix();
	part[10].ObjComputeMatrix();
	part[11].ObjComputeMatrix();
	part[10].pos = Vec3(OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz) / 15.f;
	part[11].pos = - Vec3(OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz) / 15.f;

	OpenGLLight.lightx = 0;
	OpenGLLight.lighty = cos(moveTime * timeMult / 100) * 700 - 5;
	OpenGLLight.lightz = sin(moveTime * timeMult / 100) * 700;
	OpenGLLight.lightFront = Vec3(0, -5, 0) - Vec3(OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz);
	OpenGLLight.fov = QUARTER_PI;
	OpenGLLight.nearPlane = 300.f;
	OpenGLLight.farPlane = 1000.f;

	if (coaster == true) {
		for (auto& point : hermitepoints)
		{
			point.second->obj->ObjComputeMatrix();
		}
		float min = 10000;
		for (int i = 0; i < hermitecount; ++i) {
			if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false &&
				OpenGLInput.IsLeftMouseDown() == true && showpnt) {
				Vec4 pos = OpenGLCamera.vpMat * Vec4{ hermitepoints[i]->obj->pos, 1 };
				Vec2 p = pos / pos.w;
				Vec2 m = OpenGLInput.get_mouse_pos();
				float size = glm::length(Vec2{ 2 * m.x / windowWidth - 1, 2 * m.y / windowHeight - 1 } - p);
				if (pos.z > 0 && size <= 0.3 && size < min) {
					selected = hermitepoints[i];
					min = size;
				}
				if (min == 10000)
				{
					selected = nullptr;
				}
			}
		}

		const auto next = carPos + 1 % Hdots.size();
		const auto dist = glm::length(Hdots[next].pos - Hdots[carPos].pos);
		const auto speedFactor = 0.2 / dist;
		if (carPos < lift)
		{
			timerSpeed = 0.3;
		} else if(carPos > breaker)
		{
			timerSpeed *= 0.8;
			if(timerSpeed < 0.5)
			{
				timerSpeed = 0.3;
			}
		}
		else
		{
			auto h = Hdots[carPos].pos.y - Hdots[lastPos].pos.y;
			if (h > 0)
			{
				timerSpeed -= sqrtf(2 * std::abs(h)) * 0.1;
			}
			else
			{
				timerSpeed += sqrtf(2 * std::abs(h)) * 0.1;
			}
			timerSpeed *= 0.995;
		}
		carTimer += timerSpeed * 5 * speedFactor;
		while (carTimer > 1)
		{
			carTimer--;
			carPos++;
			if (carPos == Hdots.size())
			{
				carPos = 0;
			}
		}
		while (carTimer < -1)
		{
			carTimer++;
			carPos--;
			if (carPos == Hdots.size())
			{
				carPos = 0;
			}
		}
		int lastpos = carPos;
		for (int i = 0; i < cars.size(); i++)
		{
			int pos = lastpos;
			float gap, lastgap;
			float length = 0;
			while ((gap = length - 0.7) < 0)
			{
				length += glm::length(Hdots[pos].pos - Hdots[pos - 1 < 0 ? Hdots.size() - 1 : pos - 1].pos);
				pos--;
				lastgap = gap;
				if (pos < 0)
				{
					pos = Hdots.size() + pos;
				}
			}
			if (gap + lastgap > 0)
			{
				pos--;
				if (pos < 0)
				{
					pos = Hdots.size() + pos;
				}
			}
			if (i == 2)
			{
				lastPos = pos;
			}
			if (i == 31)
			{
				breaker = pos;
			}
			Vec3 Seed = { 0, -1, 0 };
			Vec3 frontvec = normalize(Hdots[pos + 1 == Hdots.size() ? 0 : pos + 1].pos - Hdots[pos].pos);
			Vec3 newvec = Vec3(dot(Seed, frontvec) / pow(glm::length(frontvec), 2)) * Vec3(frontvec);
			Vec3 final = Seed - newvec;
			float angle = Hdots[pos].point1->angle + (Hdots[pos].point2->angle - Hdots[pos].point1->angle) * Hdots[pos].rot;
			if (Hdots[pos].point1 == hermitepoints[7]) {
				angle = Hdots[pos].point1->angle - 0.05 + (Hdots[pos].point2->angle - Hdots[pos].point1->angle - 0.05) * Hdots[pos].rot;
				angle = PI + angle;
			}
			if (Hdots[pos].point1 == hermitepoints[8] && Hdots[pos + 1].point1 != hermitepoints[9]) {
				angle = Hdots[pos].point1->angle - 0.1 + (Hdots[pos].point2->angle - Hdots[pos].point1->angle + 0.16) * Hdots[pos].rot;
				angle = PI + angle;
			}
			if (Hdots[pos].point1 == hermitepoints[6] && Hdots[pos + 1].point1 == hermitepoints[7]) {
				angle = PI + angle - 0.06;
			}
			if (Hdots[pos].point1 == hermitepoints[15] || Hdots[pos].point1 == hermitepoints[16] || Hdots[pos].point1 == hermitepoints[17] || Hdots[pos].point1 == hermitepoints[18]) {
				if (Hdots[pos + 1].point1 != hermitepoints[19]) {
					angle = PI + angle;
				}
			}
			Mat4 rotate = glm::rotate(Mat4(1.f), angle, frontvec);
			final = normalize(Vec3(rotate * Vec4(final, 0.f)));

			cars[i]->pos = Hdots[pos].pos - final * 0.8f;
			cars[i]->ObjComputeMatrix();
			lastpos = pos;
			if (i == 0 && camFixToCoaster)
			{
				if (OpenGLCamera.camMove == false) {
					OpenGLCamera.camFront = frontvec;
					OpenGLCamera.camUp = -final;
				}
				else {
					OpenGLCamera.camUp = { 0, 1, 0 };
				}
				OpenGLCamera.SetPosition((cars[0]->pos + (frontvec - final) * 0.5f) * 10.f);
				OpenGLCamera.ComputeViewProjMats();
			}
		}
	}
	if(!camFixToCoaster)
	{
		OpenGLCamera.camUp = { 0,1,0 };
	}
	for (int i = 0; i < cars.size(); ++i) {
		cars[i]->final = true;
		cars[i]->fog = fog;
		cars[i]->toon = toon;
	}
	for (int i = 0; i < pillars.size(); ++i) {
		pillars[i]->final = true;
		pillars[i]->fog = fog;
		pillars[i]->toon = toon;
	}
	for (auto& point : hermitepoints) {
		point.second->obj->fog = fog;
		point.second->obj->toon = toon;
	}
	part[9].fog = fog;
	part[9].toon = toon;
	part[10].toon = toon;
	part[11].toon = toon;
}

void FinalScene::depthSetup() {
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);
	glDisable(GL_CULL_FACE);
	depth = true;
	sea = false;
	part[9].ObjDraw(OpenGLLight.vpMat);
	if (coaster == true) {
		drawHcurve();
	}
	glFlush();
}

void FinalScene::draw() {
	depthSetup();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	GLfloat back[] = { 0.f, 0.7f * cos(moveTime * timeMult / 100), 0.9f * cos(moveTime * timeMult / 100), 1.f };
	glClearBufferfv(GL_COLOR, 0, back);
	depth = false;
	sea = false;
	part[9].ObjDraw(OpenGLCamera.vpMat);
	part[10].ObjDraw(OpenGLCamera.vpMat);
	part[11].ObjDraw(OpenGLCamera.vpMat);

	if (coaster == true) {
		drawHcurve();
	}

	sea = true;
	part[9].ObjDraw(OpenGLCamera.vpMat);

	ImGui::Begin("Final Scene");
	ImGui::Checkbox("Fog", &fog);
	ImGui::Checkbox("Toon", &toon);
	ImGui::Checkbox("Stop Time", &stop);
	if (ImGui::SliderInt("Time Speed", &timeMult, 1, 50)) {
		moveTime = 0;
	}
	if (ImGui::Checkbox("Rollercoaster", &coaster)) {
		carPos = 30;
		timerSpeed = 0;
		showpnt = false;
		showtrack = true;
	}
	if (coaster == true) {
		if (ImGui::Checkbox("Coaster Cam", &camFixToCoaster)) {
			if (camFixToCoaster == false) {
				OpenGLCamera.Reset();
			}
		}
		ImGui::Checkbox("Show Points", &showpnt);
		ImGui::Checkbox("Show Track", &showtrack);
	}
	if (ImGui::Button("Reset", ImVec2(100, 50))) {
		OpenGLCamera.Reset();
		OpenGLLight.Reset();
		OpenGLLight.lightx = 0;
		OpenGLLight.lighty = 695;
		OpenGLLight.lightz = 0;
		OpenGLLight.lightFront = Vec3(0, -5, 0) - Vec3(OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz);
		OpenGLLight.fov = QUARTER_PI;
		OpenGLLight.nearPlane = 300.f;
		OpenGLLight.farPlane = 1000.f;
		moveTime = 0;
		timeMult = 1;
		fog = false;
		toon = false;
		stop = false;
		showpnt = false;
		showtrack = true;
		coaster = false;
		carPos = 30;
		timerSpeed = 0;
	}
	ImGui::SameLine();
	if (coaster == true) {
		if (ImGui::Button("Coaster Reset", ImVec2(100, 50))) {
			carPos = 30;
			timerSpeed = 0;
			showpnt = false;
			showtrack = true;
		}
	}
	ImGui::End();
	if(selected)
	{
		ImGui::Begin("Hermite Point Info");
		ImGui::InputFloat("X Axis", &selected->obj->pos.x);
		ImGui::InputFloat("Y Axis", &selected->obj->pos.y);
		ImGui::InputFloat("Z Axis", &selected->obj->pos.z);
		ImGui::InputFloat("X Tangent", &selected->tan.x);
		ImGui::InputFloat("Y Tangent", &selected->tan.y);
		ImGui::InputFloat("Z Tangent", &selected->tan.z);
		ImGui::InputFloat("Angle", &selected->angle);
		ImGui::End();
	}
}

Vec3 FinalScene::Hermite(Point* p1, Point* p2, float t)
{
	if (t < 0 || t > 1) throw std::exception();
	float x = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * p1->obj->pos.x + (pow(t, 3) - 2 * pow(t, 2) + t) * p1->tan.x +
		(-2 * pow(t, 3) + 3 * pow(t, 2)) * p2->obj->pos.x + (pow(t, 3) - pow(t, 2)) * p2->tan.x;
	float y = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * p1->obj->pos.y + (pow(t, 3) - 2 * pow(t, 2) + t) * p1->tan.y +
		(-2 * pow(t, 3) + 3 * pow(t, 2)) * p2->obj->pos.y + (pow(t, 3) - pow(t, 2)) * p2->tan.y;
	float z = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * p1->obj->pos.z + (pow(t, 3) - 2 * pow(t, 2) + t) * p1->tan.z +
		(-2 * pow(t, 3) + 3 * pow(t, 2)) * p2->obj->pos.z + (pow(t, 3) - pow(t, 2)) * p2->tan.z;
	return Vec3{ x, y, z };
}

float FinalScene::HermiteLength(Point* p1, Point* p2)
{
	float total = 0;
	for(float t = 0; t < 1; t+= 0.05f)
	{
		float dx = (6 * pow(t, 2) - 6 * t) * p1->obj->pos.x + (3 * pow(t, 2) - 4 * t + 1) * p1->tan.x +
			(-6 * pow(t, 2) + 6 * t) * p2->obj->pos.x + (3 * pow(t, 2) - 2 * t) * p2->tan.x;
		float dy = (6 * pow(t, 2) - 6 * t) * p1->obj->pos.y + (3 * pow(t, 2) - 4 * t + 1) * p1->tan.y +
			(-6 * pow(t, 2) + 6 * t) * p2->obj->pos.y + (3 * pow(t, 2) - 2 * t) * p2->tan.y;
		float dz = (6 * pow(t, 2) - 6 * t) * p1->obj->pos.z + (3 * pow(t, 2) - 4 * t + 1) * p1->tan.z +
			(-6 * pow(t, 2) + 6 * t) * p2->obj->pos.z + (3 * pow(t, 2) - 2 * t) * p2->tan.z;
		total += sqrtf(dx * dx + dy * dy + dz * dz);
	}
	return total;
}

void FinalScene::drawHcurve()
{
	if(showpnt)
	{
		for(auto& point : hermitepoints)
		{
			point.second->obj->ObjDraw(OpenGLCamera.vpMat);
		}
	}
	updatedots();

	if (showtrack == true) {
		for (int i = 0; i < Hdots.size() - 5; i += 5) {
			int nextPoint = i + 5;
			if (Hdots.size() - 5 - i < 5) {
				nextPoint = 0;
			}
			Dot vertices[2] = { Hdots[i], Hdots[nextPoint] };
			GLuint VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Dot), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			GLuint pgm_handle = OpenGLShaderList.get_handle(11);
			glUseProgram(pgm_handle);
			GLint uniform_mvpmat = glGetUniformLocation(pgm_handle, "mvpMat");
			if (uniform_mvpmat >= 0) {
				glUniformMatrix4fv(uniform_mvpmat, 1, GL_FALSE, ValuePtr(depth ? OpenGLLight.vpMat : OpenGLCamera.vpMat));
			}
			GLint uniform_Lmodemat = glGetUniformLocation(pgm_handle, "LmodeMat");
			if (uniform_Lmodemat >= 0) {
				glUniformMatrix4fv(uniform_Lmodemat, 1, GL_FALSE, ValuePtr(OpenGLLight.vpMat));
			}
			GLint uniform_fog = glGetUniformLocation(pgm_handle, "fog");
			if (uniform_fog >= 0) {
				glUniform1i(uniform_fog, fog);
			}
			GLint uniform_toon = glGetUniformLocation(pgm_handle, "toon");
			if (uniform_toon >= 0) {
				glUniform1i(uniform_toon, toon);
			}
			Vec3 fogColor = Vec3(0.f, 0.7f * cos(moveTime * timeMult / 100), 0.9f * cos(moveTime * timeMult / 100));
			GLint uniform_fogcolor = glGetUniformLocation(pgm_handle, "fogColor");
			if (uniform_fogcolor >= 0)
			{
				glUniform3fv(uniform_fogcolor, 1, ValuePtr(fogColor));
			}

			Vec3 Seed = { 0, -1, 0 };
			Vec3 frontvec = normalize(Hdots[nextPoint].pos - Hdots[i].pos);
			Vec3 newvec = Vec3(dot(Seed, frontvec) / pow(length(frontvec), 2)) * Vec3(frontvec);
			Vec3 final = Seed - newvec;
			float angle = Hdots[i].point1->angle + (Hdots[i].point2->angle - Hdots[i].point1->angle) * Hdots[i].rot;
			if (Hdots[i].point1 == hermitepoints[7]) {
				angle = Hdots[i].point1->angle - 0.05 + (Hdots[i].point2->angle - Hdots[i].point1->angle - 0.05) * Hdots[i].rot;
				angle = PI + angle;
			}
			if (Hdots[i].point1 == hermitepoints[8] && Hdots[i + 5].point1 != hermitepoints[9]) {
				angle = Hdots[i].point1->angle - 0.1 + (Hdots[i].point2->angle - Hdots[i].point1->angle + 0.16) * Hdots[i].rot;
				angle = PI + angle;
			}
			if (Hdots[i].point1 == hermitepoints[6] && Hdots[i + 5].point1 == hermitepoints[7]) {
				angle = PI + angle - 0.06;
			}
			if (Hdots[i].point1 == hermitepoints[15] || Hdots[i].point1 == hermitepoints[16] || Hdots[i].point1 == hermitepoints[17] || Hdots[i].point1 == hermitepoints[18]) {
				if (Hdots[i + 5].point1 != hermitepoints[19]) {
					angle = PI + angle;
				}
			}
			Mat4 rotate = glm::rotate(Mat4(1.f), angle, frontvec);
			final = normalize(Vec3(rotate * Vec4(final, 0.f)));
			Vec3 left = normalize(Vec3(frontvec.y * final.z - frontvec.z * final.y, frontvec.z * final.x - frontvec.x * final.z, frontvec.x * final.y - frontvec.y * final.x)) * 0.3f;

			if(i % 200 == 0)
			{
				auto target = pillars[i / pillarFactor];
				target->pos = Hdots[i].pos + Vec3(final.x, 0, final.z) * 0.05f;
				target->scale = Vec3{ 0.2, 10 + target->pos.y,0.2 };
				if (final.y > 0)
				{
					target->pos = Vec3{ 2000000., 2000000., 2000000. };
				}
				target->pos -= Vec3{0, (10 + target->pos.y) / 2 - 0.2, 0};

				target->ObjComputeMatrix();
			}

			GLint uniform_front = glGetUniformLocation(pgm_handle, "front");
			if (uniform_front >= 0) {
				glUniform3fv(uniform_front, 1, ValuePtr(frontvec));
			}
			GLint uniform_final = glGetUniformLocation(pgm_handle, "final");
			if (uniform_final >= 0) {
				glUniform3fv(uniform_final, 1, ValuePtr(final));
			}
			GLint uniform_left = glGetUniformLocation(pgm_handle, "left");
			if (uniform_left >= 0) {
				glUniform3fv(uniform_left, 1, ValuePtr(left));
			}
			 
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, fScene.depthMap);
			GLuint textureLoc = glGetUniformLocation(pgm_handle, "depthMap");
			glUniform1i(textureLoc, 1);
			Vec3 Light = -Normalize(OpenGLLight.lightFront);
			GLint uniform_light = glGetUniformLocation(pgm_handle, "light");
			if (uniform_light >= 0)
			{
				glUniform3fv(uniform_light, 1, ValuePtr(Light));
			}

			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 0, 2);
			glUseProgram(0);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}
		for (auto pillar : pillars)
		{
			pillar->ObjDraw(depth ? OpenGLLight.vpMat : OpenGLCamera.vpMat);
		}
	}
	for(auto car : cars)
	{
		car->ObjDraw(depth ? OpenGLLight.vpMat : OpenGLCamera.vpMat);
	}
}

void FinalScene::updatedots()
{
	Hdots.clear();
	for (int i = 0; i < hermitecount - 1; ++i) {
		int linecount = HermiteLength(hermitepoints[i], hermitepoints[i + 1]);
		for (int j = 1; j <= linecount; ++j) {
			float t = static_cast<float>(j) / linecount;
			Vec3 dot = Hermite(hermitepoints[i], hermitepoints[i + 1], t);
			Hdots.push_back(Dot{ Vec3{ dot.x, dot.y , dot.z }, hermitepoints[i], hermitepoints[i + 1], t });
		}
		if(i == 4)
		{
			lift = Hdots.size();
		}
		else if(i == 31)
		{
			breaker = Hdots.size();
		}
	}

	int linecount = HermiteLength(hermitepoints[hermitecount - 1], hermitepoints[0]);
	for (int j = 1; j <= linecount; ++j) {
		float t = static_cast<float>(j) / linecount;
		Vec3 dot = Hermite(hermitepoints[hermitecount - 1], hermitepoints[0], t);
		Hdots.push_back(Dot{ Vec3{ dot.x, dot.y , dot.z }, hermitepoints[hermitecount - 1], hermitepoints[0], t });
	}
}

void FinalScene::AddPoint(Vec3 pos, Vec3 tan, float angle)
{
	Object* point = new Object(SPHERE, 0, Vec4(0.5f, 1.f, 1.f, 1.f), false);
	point->pos = pos;
	point->final = true;
	hermitepoints[hermitecount] = new Point(point);
	hermitepoints[hermitecount]->tan = tan;
	hermitepoints[hermitecount]->angle = angle;
	hermitecount++;
}
