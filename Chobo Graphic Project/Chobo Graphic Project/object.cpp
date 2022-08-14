#define GLM_FORCE_SILENT_WARNINGS
#include "OpenGL.h"
#include "object.h"

void Object::ObjComputeMatrix()
{
	Mat4 xR = Rotate(rotx, glm::vec3{ 1.f, 0.f, 0.f });
	Mat4 yR = Rotate(roty, glm::vec3{ 0.f, 1.f, 0.f });
	Mat4 zR = Rotate(rotz, glm::vec3{ 0.f, 0.f, 1.f });
	rotMat = Translate(pos) * xR * yR * zR * Scale(scale);
	nrmMat = transpose(inverse(rotMat));
}

void Object::reset()
{
	rotx = 0.f;
	roty = 0.f;
	rotz = 0.f;
}

void Object::ObjDraw(const Mat4& MVPmat)
{
	GLuint map;
	if (whichShader == 7) {
		glGenTextures(1, &map);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, map);
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, imageWidth, imageHeight, 0, GL_RED, GL_FLOAT, mScene.noiseMap);
		glBindTexture(GL_TEXTURE_2D, map);
	}
	if (whichShader == 10) {
		glGenTextures(1, &map);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, map);
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, imageWidth, imageHeight, 0, GL_RED, GL_FLOAT, fScene.grassMap);
		glBindTexture(GL_TEXTURE_2D, map);
	}

	GLuint pgm_handle = OpenGLShaderList.get_handle(whichShader);
	glUseProgram(pgm_handle);

	if (whichShader == 8) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sScene.depthMap);
		GLuint textureLoc = glGetUniformLocation(pgm_handle, "depthMap");
		glUniform1i(textureLoc, GL_TEXTURE0);
	}
	if (whichShader == 10) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, fScene.depthMap);
		GLuint textureLoc = glGetUniformLocation(pgm_handle, "depthMap");
		glUniform1i(textureLoc, 1);
	}

	GLint uniform_color = glGetUniformLocation(pgm_handle, "color");
	if (uniform_color >= 0)
	{
		glUniform4fv(uniform_color, 1, ValuePtr(color));
	}
	else
	{
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	Vec3 Light = Normalize(Vec3{ OpenGLLight.lightx, OpenGLLight.lighty, OpenGLLight.lightz });
	if (OpenGLLight.shadow == true) {
		Light = -Normalize(OpenGLLight.lightFront);
	}
	GLint uniform_light = glGetUniformLocation(pgm_handle, "light");
	if (uniform_light >= 0)
	{
		glUniform3fv(uniform_light, 1, ValuePtr(Light));
	}
	else
	{
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_mvpmat = glGetUniformLocation(pgm_handle, "mvpMat");
	if (uniform_mvpmat >= 0) {
		glUniformMatrix4fv(uniform_mvpmat, 1, GL_FALSE, ValuePtr(MVPmat));
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_rotmat = glGetUniformLocation(pgm_handle, "rotMat");
	if (uniform_rotmat >= 0) {
		glUniformMatrix4fv(uniform_rotmat, 1, GL_FALSE, ValuePtr(rotMat));
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	if (light == true) {
		nrmMat = OpenGLLight.ltnmMat;
	}
	GLint uniform_nrmmat = glGetUniformLocation(pgm_handle, "nrmMat");
	if (uniform_nrmmat >= 0) {
		glUniformMatrix4fv(uniform_nrmmat, 1, GL_FALSE, ValuePtr(nrmMat));
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_islight = glGetUniformLocation(pgm_handle, "islight");
	if (uniform_islight >= 0) {
		glUniform1i(uniform_islight, light);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_toon = glGetUniformLocation(pgm_handle, "toon");
	if (uniform_toon >= 0) {
		glUniform1i(uniform_toon, toon);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_fog = glGetUniformLocation(pgm_handle, "fog");
	if (uniform_fog >= 0) {
		glUniform1i(uniform_fog, fog);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	Vec3 fogColor = { mScene.fogcolor[0], mScene.fogcolor[1], mScene.fogcolor[2] };
	GLint uniform_fogcolor = glGetUniformLocation(pgm_handle, "fogColor");
	if (uniform_fogcolor >= 0)
	{
		if (final == true) {
			fogColor = Vec3( 0.f, 0.7f * cos(fScene.moveTime * fScene.timeMult / 100), 0.9f * cos(fScene.moveTime * fScene.timeMult / 100) );
			glUniform3fv(uniform_fogcolor, 1, ValuePtr(fogColor));
		}
		else {
			glUniform3fv(uniform_fogcolor, 1, ValuePtr(fogColor));
		}
	}
	else
	{
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_mindist = glGetUniformLocation(pgm_handle, "Mindist");
	if (uniform_mindist >= 0) {
		if (final == true) {
			glUniform1f(uniform_mindist, fScene.mindist);
		}
		else {
			glUniform1f(uniform_mindist, mScene.mindist);
		}
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_maxdist = glGetUniformLocation(pgm_handle, "Maxdist");
	if (uniform_maxdist >= 0) {
		if (final == true) {
			glUniform1f(uniform_maxdist, fScene.maxdist);
		}
		else {
			glUniform1f(uniform_maxdist, mScene.maxdist);
		}
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_shrink = glGetUniformLocation(pgm_handle, "uShrink");
	if (uniform_shrink >= 0) {
		glUniform1f(uniform_shrink, mScene.shrink);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_inner = glGetUniformLocation(pgm_handle, "TessLevelInner");
	if (uniform_inner >= 0) {
		glUniform1f(uniform_inner, mScene.TessLevelInner);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_outer = glGetUniformLocation(pgm_handle, "TessLevelOuter");
	if (uniform_outer >= 0) {
		glUniform1f(uniform_outer, mScene.TessLevelOuter);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_sphere = glGetUniformLocation(pgm_handle, "sphere");
	if (uniform_sphere >= 0) {
		glUniform1i(uniform_sphere, mScene.sphere);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_mesh = glGetUniformLocation(pgm_handle, "mesh");
	if (uniform_mesh >= 0) {
		glUniform1i(uniform_mesh, meshID);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_explode = glGetUniformLocation(pgm_handle, "explode");
	if (uniform_explode >= 0) {
		glUniform1f(uniform_explode, mScene.explode);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_Lmodemat = glGetUniformLocation(pgm_handle, "LmodeMat");
	if (uniform_Lmodemat >= 0) {
		glUniformMatrix4fv(uniform_Lmodemat, 1, GL_FALSE, ValuePtr(OpenGLLight.vpMat));
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_depth = glGetUniformLocation(pgm_handle, "depth");
	if (uniform_depth >= 0) {
		glUniform1i(uniform_depth, sScene.depth);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_backlight = glGetUniformLocation(pgm_handle, "backlight");
	if (uniform_backlight >= 0) {
		glUniform1i(uniform_backlight, sScene.backlight);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_sea = glGetUniformLocation(pgm_handle, "sea");
	if (uniform_sea >= 0) {
		glUniform1i(uniform_sea, fScene.sea);
	}
	else {
		//throw std::exception("Uniform variable doesn't exist!!!\n");
	}

	SendVertexData(meshes[meshID]);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glBindVertexArray(meshes[meshID].VAO);
	glDrawElements(primitive, meshes[meshID].numIndices, GL_UNSIGNED_INT, nullptr);
	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteTextures(1, &map);
	glDeleteVertexArrays(1, &meshes[meshID].VAO);
	glDeleteBuffers(1, &meshes[meshID].VBO);
	glDeleteBuffers(1, &meshes[meshID].IBO);
}

void Object::ObjNoiseDraw(const Mat4& MVPmat, float* noiseMap)
{
	GLuint map;
	glGenTextures(1, &map);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, map);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, imageWidth, imageHeight, 0, GL_RED, GL_FLOAT, noiseMap);
	glBindTexture(GL_TEXTURE_2D, map);

	GLuint pgm_handle = OpenGLShaderList.get_handle(whichShader);
	glUseProgram(pgm_handle);

	GLuint textureLoc = glGetUniformLocation(pgm_handle, "noise");
	glUniform1i(textureLoc, GL_TEXTURE0);

	GLint uniform_mvpmat = glGetUniformLocation(pgm_handle, "mvpMat");
	if (uniform_mvpmat >= 0) {
		glUniformMatrix4fv(uniform_mvpmat, 1, GL_FALSE, ValuePtr(MVPmat));
	}
	else {
		throw std::exception("Uniform variable doesn't exist!!!\n");
	}
	GLint uniform_rotmat = glGetUniformLocation(pgm_handle, "rotMat");
	if (uniform_rotmat >= 0) {
		glUniformMatrix4fv(uniform_rotmat, 1, GL_FALSE, ValuePtr(rotMat));
	}
	else {
		throw std::exception("Uniform variable doesn't exist!!!\n");
	}

	SendVertexData(meshes[meshID]);
	glBindVertexArray(meshes[meshID].VAO);
	glDrawElements(GL_TRIANGLES, meshes[meshID].numIndices, GL_UNSIGNED_INT, nullptr);
	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteTextures(1, &map);
	glDeleteVertexArrays(1, &meshes[meshID].VAO);
	glDeleteBuffers(1, &meshes[meshID].VBO);
	glDeleteBuffers(1, &meshes[meshID].IBO);
}

void Object::MouseObj()
{
	Vec2 increment = Vec2(OpenGLInput.get_mouse_pos() - OpenGLInput.get_mouse_previous_pos());
	if (increment != Vec2{ 0, 0 }) {
		rotx -= increment.y / 100;
		roty += increment.x / 100;
	}
}

void Object::UpdateMesh(int count, int stack, int slice, float angle1, float angle2) {
	switch (count) {
	case (PartID::PLA):
		meshes[part[count].meshID] = CreatePlane(stack, slice);
		part[count].stack = stack;
		part[count].slice = slice;
		break;
	case (PartID::CUB):
		meshes[part[count].meshID] = CreateCube(stack, slice);
		part[count].stack = stack;
		part[count].slice = slice;
		break;
	case (PartID::SPH):
		meshes[part[count].meshID] = CreateSphere(stack, slice);
		part[count].stack = stack;
		part[count].slice = slice;
		break;
	case (PartID::CYL):
		meshes[part[count].meshID] = CreateCylinder(stack, slice);
		part[count].stack = stack;
		part[count].slice = slice;
		break;
	case (PartID::CON):
		meshes[part[count].meshID] = CreateCone(stack, slice);
		part[count].stack = stack;
		part[count].slice = slice;
		break;
	case (PartID::TOR):
		meshes[part[count].meshID] = CreateTorus(stack, slice, angle1, angle2);
		part[count].stack = stack;
		part[count].slice = slice;
		part[count].angle1 = angle1;
		part[count].angle2 = angle2;
		break;
	default:
		break;
	}
}