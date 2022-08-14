#include "OpenGL.h"
#include "noise.h"
#include "object.h"
#include <functional>

void NoiseScene::update()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	for (unsigned j = 0; j < imageHeight; ++j) {
		for (unsigned i = 0; i < imageWidth; ++i) {
			noiseMap[j * imageWidth + i] = 0;
		}
	}
	ValueNoise noise;
	noise.SetSmooth(smoothmethod);
	PerlinNoise Pnoise;
	Pnoise.SetSmooth(smoothmethod);
	switch (noisetype) {
	case White:
	{
		unsigned seed = 2016;
		std::mt19937 gen(seed);
		std::uniform_real_distribution<float> distr;
		auto dice = std::bind(distr, gen); // std::function<float()> 

		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				// generate a float in the range [0:1]
				noiseMap[j * imageWidth + i] = dice();
			}
		}
	}
	break;
	case Value:
	{
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				// generate a float in the range [0:1]
				Vec2 pNoise = Vec2(i, j) * frequency + Vec2(moveTime, 0);
				noiseMap[j * imageWidth + i] = noise.eval(pNoise);
			}
		}
	}
	break;
	case Fractal:
	{
		float maxNoiseVal = 0;
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec2 pNoise = Vec2(i, j) * frequency + Vec2(moveTime, 0);
				float amplitude = 1;
				for (unsigned l = 0; l < static_cast<unsigned>(numLayers); ++l) {
					noiseMap[j * imageWidth + i] += noise.eval(pNoise) * amplitude;
					pNoise *= frequencyMult;
					amplitude *= amplitudeMult;
				}
				if (noiseMap[j * imageWidth + i] > maxNoiseVal) {
					maxNoiseVal = noiseMap[j * imageWidth + i];
				}
			}
		}
		for (unsigned i = 0; i < imageWidth * imageHeight; ++i) {
			noiseMap[i] /= maxNoiseVal;
		}
	}
	break;
	case Turbulence:
	{
		float maxNoiseVal = 0;
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec2 pNoise = Vec2(i, j) * frequency + Vec2(moveTime, 0);
				float amplitude = 1;
				for (unsigned l = 0; l < static_cast<unsigned>(numLayers); ++l) {
					noiseMap[j * imageWidth + i] += std::fabs(2 * noise.eval(pNoise) - 1) * amplitude;
					pNoise *= frequencyMult;
					amplitude *= amplitudeMult;
				}
				if (noiseMap[j * imageWidth + i] > maxNoiseVal) {
					maxNoiseVal = noiseMap[j * imageWidth + i];
				}
			}
		}
		for (unsigned i = 0; i < imageWidth * imageHeight; ++i) {
			noiseMap[i] /= maxNoiseVal;
		}
	}
	break;
	case Marble:
	{
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec2 pNoise = Vec2(i, j) * frequency + Vec2(moveTime, 0);
				float amplitude = 1;
				float noiseValue = 0;
				// compute some fractal noise
				for (unsigned l = 0; l < static_cast<unsigned>(numLayers); ++l) {
					noiseValue += noise.eval(pNoise) * amplitude;
					pNoise *= frequencyMult;
					amplitude *= amplitudeMult;
				}
				// we "displace" the value i used in the sin() expression by noiseValue * 100
				noiseMap[j * imageWidth + i] = (sin((i + noiseValue * 100) * 2 * PI / 200.f) + 1) / 2.f;
			}
		}
	}
	break;
	case Wood:
	{
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec2 pNoise = Vec2(i, j) * frequency + Vec2(moveTime, 0);
				float g = noise.eval(pNoise) * 10;
				noiseMap[j * imageWidth + i] = g - (int)g;
			}
		}
	}
	break;
	case Gradient:
	{
		Vec3 derivs = { 0, 0, 0 };
		for (unsigned j = 0; j < imageHeight; ++j) {
			for (unsigned i = 0; i < imageWidth; ++i) {
				Vec3 pNoise = Vec3(i, 0, j) * frequency + Vec3(moveTime, 0, 0);
				float g = Pnoise.eval(pNoise, derivs);
				noiseMap[j * imageWidth + i] = (g + 1) * 0.5f;
			}
		}
	}
	break;
	}
	if (mapping == false) {
		mat = OpenGLCamera.vpMat * Rotate(PI, { 1, 0, 0 }) * Scale(Vec3(magnify));
		for (int i = 0; i < meshes[part[8].meshID].numVertices; ++i) {
			meshes[part[8].meshID].vertexBuffer[i].pos.z = 0.f;
		}
	}
	else {
		mat = OpenGLCamera.vpMat * Rotate(HALF_PI, { 1, 0, 0 });
		if (terraceMode == true) {
			for (int i = 0; i < meshes[part[8].meshID].numVertices; ++i) {
				meshes[part[8].meshID].vertexBuffer[i].pos.z = 0.5f - round(noiseMap[i] * (terrace * 10) / 4) / (terrace * 10);
			}
		}
		else {
			for (int i = 0; i < meshes[part[8].meshID].numVertices; ++i) {
				meshes[part[8].meshID].vertexBuffer[i].pos.z = 0.5f - noiseMap[i] / 4;
			}
		}
	}
	if (mapping == true) {
		if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false) {
			if (OpenGLInput.IsLeftMouseDown() == true) {
				Vec2 increment = Vec2(OpenGLInput.get_mouse_pos() - OpenGLInput.get_mouse_previous_pos());
				if (increment != Vec2{ 0, 0 }) {
					part[8].rotx -= increment.y / 100;
					part[8].roty -= increment.x / 100;
				}
			}
		}
		part[8].rotz += OpenGLInput.get_mouse_scroll_value() / 5.f;
	}
	else {
		part[8].reset();
	}
	part[8].ObjComputeMatrix();
}

void NoiseScene::draw()
{
	OpenGLCamera.ComputeViewProjMats();
	ImGui::Begin("Value Noise");
	const char* types[] = { "White", "Value", "Fractal", "Turbulence", "Marble", "Wood", "Gradient" };
	ImGui::Combo("Noise Type", &noisetype, types, IM_ARRAYSIZE(types));
	const char* methods[] = { "Linear", "Cosine", "SmoothStep", "Quintic" };
	ImGui::Combo("Smooth Method", &smoothmethod, methods, IM_ARRAYSIZE(methods));
	ImGui::SliderFloat("Frequency", &frequency, 0.001f, 0.2f);
	ImGui::SliderFloat("FrequencyMult", &frequencyMult, 0.f, 10.f);
	ImGui::SliderFloat("AmplitudeMult", &amplitudeMult, 0.f, 1.f);
	ImGui::SliderInt("NumLayers", &numLayers, 1, 8);
	ImGui::SliderFloat("Magnify", &magnify, 0.5f, 3.f);
	ImGui::Checkbox("Terrace Mode", &terraceMode);
	if (terraceMode == true) {
		ImGui::SliderInt("Terrace", &terrace, 1, 10);
	}
	if (ImGui::Button("Mapping", ImVec2(80, 40))) {
		OpenGLCamera.Reset();
		mapping = !mapping;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset", ImVec2(80, 40))) {
		frequency = 0.02f;
		frequencyMult = 1.8f;
		amplitudeMult = 0.35f;
		numLayers = 5;
		magnify = 1.f;
		part[8].reset();
		OpenGLCamera.Reset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Anim", ImVec2(80, 40))) {
		isMoved = !isMoved;
	}
	ImGui::End();
	if (isMoved)
	{
		moveTime += ImGui::GetIO().DeltaTime;
	}
	part[8].ObjNoiseDraw(mat, noiseMap);
}
