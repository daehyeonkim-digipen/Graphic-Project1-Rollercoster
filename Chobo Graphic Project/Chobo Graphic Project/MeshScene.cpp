#include "OpenGL.h"
#include "object.h"
#include "noise.h"

void MeshScene::update()
{
	if (wire == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (backcull == true) {
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
	}
	if (grid == false) {
		if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false) {
			if (OpenGLInput.IsLeftMouseDown() == true) {
				part[count].MouseObj();
			}
		}
		part[count].rotz += OpenGLInput.get_mouse_scroll_value() / 5.f;
	}
	part[count].color.r = col[0];
	part[count].color.g = col[1];
	part[count].color.b = col[2];
	if (mindist >= maxdist) {
		maxdist = mindist;
	}
	switch (geoshad) {
	case(GeoShad::None):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 0;
			part[i].primitive = GL_TRIANGLES;
		}
		break;
	case(GeoShad::Shrink):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 3;
			part[i].primitive = GL_TRIANGLES;
		}
		break;
	case(GeoShad::Bezier):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 4;
			part[i].primitive = GL_LINES_ADJACENCY;
		}
		break;
	case(GeoShad::Explode):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 5;
			part[i].primitive = GL_TRIANGLES;
		}
		break;
	case(GeoShad::Tessellation):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 6;
			part[i].primitive = GL_PATCHES;
		}
		break;
	case(GeoShad::Grass):
		for (int i = 0; i < 7; ++i) {
			part[i].whichShader = 7;
			part[i].primitive = GL_PATCHES;
			part[i].color = Vec4(0.0, 0.4, 0.0, 1.0);
		}
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				noiseMap[j * imageWidth + i] = 0;
			}
		}
		PerlinNoise Pnoise;
		Pnoise.SetSmooth(SmoothMethod::Quintic);
		Vec3 derivs = { 0, 0, 0 };
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec3 pNoise = Vec3(i, 0, j) * 0.01f + Vec3(moveTime, 0, 0);
				float g = Pnoise.eval(pNoise, derivs);
				noiseMap[j * imageWidth + i] = (g + 1) * 0.5f;
			}
		}
		if (move == true) {
			moveTime += ImGui::GetIO().DeltaTime;
		}
		break;
	}
	if (meshchanged == true) {
		for (auto obj : Objs) {
			delete obj;
		}
		Objs.clear();
		for (int i = 0; i < gridsize; ++i) {
			for (int j = 0; j < gridsize; ++j) {
				if (i == gridsize / 2 && j == gridsize / 2) {
					continue;
				}
				Objs.push_back(new Object(part[count].meshID, 0, { 1.f, 1.f, 1.f, 1.f }));
				Objs.back()->pos = { static_cast<float>(i - gridsize / 2) * 3 / 2, static_cast<float>(j - gridsize / 2) * 3 / 2, 0 };
				Objs.back()->ObjComputeMatrix();
			}
		}
		meshchanged = false;
	}
}

void MeshScene::draw()
{
	OpenGLCamera.ComputeViewProjMats();
	OpenGLLight.ComputeLightMats();
	part[LIGHT].toon = part[count].toon;
	part[LIGHT].fog = part[count].fog;
	part[LIGHT].ObjDraw(OpenGLLight.litMat);
	part[count].ObjComputeMatrix();
	part[count].ObjDraw(OpenGLCamera.vpMat);
	ImGui::Begin("Procedural Modeling");
	ImGui::Checkbox("Wireframe Mode", &wire);
	ImGui::Checkbox("Enable Backcull", &backcull);
	ImGui::Checkbox("Toon Shading", &part[count].toon);
	ImGui::Checkbox("Apply Fog", &part[count].fog);
	if (ImGui::Checkbox("Grid Mode", &grid)) {
		if (grid == true) {
			part[count].reset();
			OpenGLCamera.Reset();
			OpenGLLight.SetLightVec(gridsize * 15);
			OpenGLCamera.SetPosition({ 0, 0, 100 });
			OpenGLCamera.SetCamSpeed(1.f);
			mindist = 0;
			maxdist = 150;
			delay = true;
		}
		else {
			OpenGLCamera.Reset();
			OpenGLCamera.SetPosition({ 0, 0, 20 });
			OpenGLCamera.SetCamSpeed(0.15f);
			mindist = 5;
			maxdist = 20;
		}
	}
	if (grid == false) {
		if (count <= PartID::CUB) {
			ImGui::SliderInt("Stacks", &stack, 1, 32);
			ImGui::SliderInt("Slices", &slice, 1, 32);
		}
		else {
			ImGui::SliderInt("Stacks", &stack, 4, 64);
			ImGui::SliderInt("Slices", &slice, 4, 64);
		}
		if (count == PartID::TOR) {
			ImGui::SliderFloat("Start Angle", &angle1, 0, angle2 - SIXTEENTH_PI);
			ImGui::SliderFloat("End Angle", &angle2, angle1 + SIXTEENTH_PI, TWO_PI);
		}
		ImGui::SliderFloat("Fog Min Dist", &mindist, 0.f, 20.f);
		ImGui::SliderFloat("Fog Max Dist", &maxdist, 0.f, 20.f);
		part[count].UpdateMesh(count, stack, slice, angle1, angle2);
	}
	else {
		if (ImGui::SliderInt("Grid Size", &gridsize, 1, 36)) {
			meshchanged = true;
			OpenGLLight.SetLightVec(gridsize * 15);
		}
		ImGui::SliderFloat("Fog Min Dist", &mindist, 0.f, 150.f);
		ImGui::SliderFloat("Fog Max Dist", &maxdist, 0.f, 150.f);
		if (delay == false) {
			for (auto obj : Objs) {
				obj->toon = part[count].toon;
				obj->fog = part[count].fog;
				obj->color = part[count].color;
				obj->whichShader = part[count].whichShader;
				obj->primitive = part[count].primitive;
				obj->ObjDraw(OpenGLCamera.vpMat);
			}
		}
		delay = false;
	}
	ImGui::SliderFloat("light.x", &OpenGLLight.lightx, -1.f, 1.f);
	ImGui::SliderFloat("light.y", &OpenGLLight.lighty, -1.f, 1.f);
	ImGui::SliderFloat("light.z", &OpenGLLight.lightz, -1.f, 1.f);
	if (ImGui::Button("Meshes", ImVec2(100, 50))) {
		count++;
		if (geoshad != GeoShad::Grass) {
			col[0] = part[count].color.r;
			col[1] = part[count].color.g;
			col[2] = part[count].color.b;
		}
		if (grid == true) {
			part[count].reset();
		}
		stack = part[count].stack;
		slice = part[count].slice;
		angle1 = part[count].angle1;
		angle2 = part[count].angle2;
		meshchanged = true;
	}
	if (count >= 7) {
		count = 0;
		if (geoshad != GeoShad::Grass) {
			col[0] = part[count].color.r;
			col[1] = part[count].color.g;
			col[2] = part[count].color.b;
		}
		if (grid == true) {
			part[count].reset();
		}
		stack = part[count].stack;
		slice = part[count].slice;
		angle1 = part[count].angle1;
		angle2 = part[count].angle2;
		meshchanged = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(100, 50))) {
		mindist = 5.f;
		maxdist = 20.f;
		OpenGLCamera.Reset();
		part[count].reset();
		fogcolor[0] = 0.f;
		fogcolor[1] = 0.f;
		fogcolor[2] = 0.f;
		grid = false;
		OpenGLLight.SetLightVec(15);
		meshchanged = true;
	}
	ImGui::End();
	ImGui::Begin("Mesh Color");
	if (ImGui::ColorPicker3("Color", col)) {
		meshchanged = true;

	}
	ImGui::End();
	ImGui::Begin("Fog Color");
	ImGui::ColorPicker3("Color", fogcolor);
	ImGui::End();
	ImGui::Begin("Geometry/Tessellation Shader");
	const char* types[] = { "None", "Shrink", "Bezier", "Explode", "Tessellation", "Grass"};
	if (ImGui::Combo("Shader Type", &geoshad, types, IM_ARRAYSIZE(types))) {
		if (geoshad != GeoShad::Explode) {
			OpenGLLight.SetLightVec(15);
		}
		if (geoshad != GeoShad::Grass) {
			for (int i = 0; i < 7; ++i) {
				part[i].color.r = col[0];
				part[i].color.g = col[1];
				part[i].color.b = col[2];
			}
		}
	}
	if (geoshad == GeoShad::Shrink) {
		ImGui::SliderFloat("Size", &shrink, 0.f, 1.f);
	}
	if (geoshad == GeoShad::Explode) {
		ImGui::SliderFloat("amount", &explode, 0.f, 2.f);
		OpenGLLight.SetLightVec(30);
	}
	if (geoshad == GeoShad::Tessellation) {
		ImGui::SliderInt("Level Inner", &TessLevelInner, 1, 32);
		ImGui::SliderInt("Level Outer", &TessLevelOuter, 1, 32);
		ImGui::SliderFloat("Size", &shrink, 0.f, 1.f);
		ImGui::Checkbox("Sphere Mode", &sphere);
	}
	if (geoshad == GeoShad::Grass) {
		ImGui::SliderInt("Level Inner", &TessLevelInner, 1, 32);
		ImGui::SliderInt("Level Outer", &TessLevelOuter, 1, 32);
		ImGui::Checkbox("Wind Effect", &move);
	}
	if (ImGui::Button("Reset", ImVec2(100, 50))) {
		shrink = 0.95f;
		explode = 0;
		TessLevelInner = 1;
		TessLevelOuter = 1;
		sphere = true;
		move = false;
	}
	ImGui::End();
}