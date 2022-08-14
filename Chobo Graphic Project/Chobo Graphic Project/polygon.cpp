#define GLM_FORCE_SILENT_WARNINGS
#include <iostream>
#include "polygon.h"
#include "OpenGL.h"

void Polygon::linesetup() {
	Polygon::GLModel glmodel;
	std::vector<glm::vec2> pos_vtx{ {0, 0}, {1, 0} };
	GLuint pos_cnt = static_cast<int>(pos_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = pos_cnt;
	glmodel.primitive_type = GL_LINES;
	glmodel.primitive_cnt = pos_cnt / 2;
	models["line"] = glmodel;
}

void Polygon::trisetup() {
	Polygon::GLModel glmodel;
	std::vector<glm::vec2> pos_vtx{ {0, 0}, {1, 0}, {0, 1} };
	std::vector<GLushort> idx_vtx{ 0, 1, 2 };
	GLuint idx_elem_cnt = static_cast<int>(idx_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = idx_elem_cnt;
	glmodel.primitive_type = GL_TRIANGLES;
	glmodel.primitive_cnt = idx_elem_cnt - 2;
	models["triangle"] = glmodel;
}

void Polygon::ellipsetup() {
	Polygon::GLModel glmodel;
	std::vector<glm::vec2> pos_vtx;
	std::vector<GLushort> idx_vtx;
	double slice = PI / 25;
	pos_vtx.push_back({ 0, 0 });
	for (int i = 0; i < 50; i++) {
		double x1 = cos(slice * i);
		double y1 = sin(slice * i);
		pos_vtx.push_back({ x1, y1 });
	}
	for (GLushort j = 0; j <= 50; j++) {
		idx_vtx.push_back(j);
	}
	idx_vtx.push_back(1);
	GLuint idx_elem_cnt = static_cast<int>(idx_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = idx_elem_cnt;
	glmodel.primitive_type = GL_TRIANGLE_FAN;
	glmodel.primitive_cnt = idx_elem_cnt - 2;
	models["ellipse"] = glmodel;
}

void Polygon::GLObject::draw() {
	GLuint shd_hdl = OpenGLShaderList.get_handle(2);
	glUseProgram(shd_hdl);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mdl_to_ndc_xform = OpenGLPolygon.NDC * xform * mdl_xform;

	GLint uniform_var_loc1 = glGetUniformLocation(shd_hdl, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) {
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, ValuePtr(OpenGLPolygon.globject.mdl_to_ndc_xform));
	}
	else {
		std::cout << "uModel_to_NDC variable doesn't exist!!!\n";
	}
	GLint uniform_var_loc2 = glGetUniformLocation(shd_hdl, "uColor");
	if (uniform_var_loc2 >= 0) {
		glUniform4fv(uniform_var_loc2, 1, ValuePtr(OpenGLPolygon.globject.color));
	}
	else {
		std::cout << "uColor variable doesn't exist!!!\n";
	}

	glBindVertexArray(OpenGLPolygon.models[mdl_ref].vaoid);
	if (OpenGLPolygon.models[mdl_ref].primitive_type == GL_LINES) {
		glLineWidth(width);
		glDrawArrays(GL_LINES, 0,OpenGLPolygon.models[mdl_ref].draw_cnt);
		OpenGLPolygon.globject.width = 1.f;
		glLineWidth(1.f);
	}
	else if (OpenGLPolygon.models[mdl_ref].primitive_type == GL_TRIANGLES) {
		glLineWidth(1.f);
		glDrawElements(GL_TRIANGLES, OpenGLPolygon.models[mdl_ref].draw_cnt, GL_UNSIGNED_SHORT, NULL);
		glLineWidth(1.f);
	}
	else if (OpenGLPolygon.models[mdl_ref].primitive_type == GL_TRIANGLE_FAN) {
		glLineWidth(1.f);
		glDrawElements(GL_TRIANGLE_FAN, OpenGLPolygon.models[mdl_ref].draw_cnt, GL_UNSIGNED_SHORT, NULL);
		glLineWidth(1.f);
	}
	glUseProgram(0);
}

void set_line_width(double width) {
	OpenGLPolygon.glsetting.LW = static_cast<float>(width);
}

void set_line_color(double r, double g, double b, double a) {
	OpenGLPolygon.glsetting.LR = static_cast<float>(r) / 255.f;
	OpenGLPolygon.glsetting.LG = static_cast<float>(g) / 255.f;
	OpenGLPolygon.glsetting.LB = static_cast<float>(b) / 255.f;
	OpenGLPolygon.glsetting.LA = static_cast<float>(a) / 255.f;
}

void no_outline() {
	OpenGLPolygon.glsetting.noline = GL_TRUE;
}

void yes_outline() {
	OpenGLPolygon.glsetting.noline = GL_FALSE;
}

void no_fill() {
	OpenGLPolygon.glsetting.nofill = GL_TRUE;
}

void yes_fill() {
	OpenGLPolygon.glsetting.nofill = GL_FALSE;
}

void set_fill_color(double r, double g, double b, double a) {
	OpenGLPolygon.glsetting.FR = static_cast<float>(r) / 255.f;
	OpenGLPolygon.glsetting.FG = static_cast<float>(g) / 255.f;
	OpenGLPolygon.glsetting.FB = static_cast<float>(b) / 255.f;
	OpenGLPolygon.glsetting.FA = static_cast<float>(a) / 255.f;
}

void draw_line(double x1, double y1, double x2, double y2) {
	glm::vec2 length = { x2 - x1, y2 - y1 };
	double angle = atan(length.y / length.x);
	double len = 0;
	if (length.x < 0) {
		len = -sqrt(pow(length.x, 2) + pow(length.y, 2));
	}
	else {
		len = sqrt(pow(length.x, 2) + pow(length.y, 2));
	}
	glm::mat3 R = { {cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1} };
	glm::mat3 S = { {len, 0, 0}, {0, len, 0}, {0, 0, 1} };
	glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x1, y1, 1} };
	OpenGLPolygon.globject.color = { OpenGLPolygon.glsetting.LR, OpenGLPolygon.glsetting.LG, OpenGLPolygon.glsetting.LB, OpenGLPolygon.glsetting.LA };
	OpenGLPolygon.globject.width = OpenGLPolygon.glsetting.LW;
	OpenGLPolygon.globject.mdl_xform = T * R * S;
	OpenGLPolygon.globject.xform = OpenGLPolygon.glsetting.translatemat * OpenGLPolygon.glsetting.rotatemat * OpenGLPolygon.glsetting.scalemat;
	OpenGLPolygon.globject.mdl_ref = "line";
	OpenGLPolygon.globject.draw();
}

void draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
	if (OpenGLPolygon.glsetting.nofill == GL_FALSE) {
		if (((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) <= 0) {
			std::swap(x2, x3);
			std::swap(y2, y3);
		}
		glm::vec2 length12 = { x2 - x1, y2 - y1 };
		glm::vec2 length13 = { x3 - x1, y3 - y1 };
		double inc = length12.y / length12.x;
		double angle = atan(inc);
		if (length12.x == 0) {
			angle = PI / 2;
		}
		if ((angle < 0 && length12.y > 0) || (angle > 0 && length12.y < 0)) {
			angle = PI + angle;
		}
		glm::mat3 R = { {cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1} };
		glm::mat3 IR = { {cos(angle), -sin(angle), 0}, {sin(angle), cos(angle), 0}, {0, 0, 1} };
		glm::mat3 pos1 = IR * glm::mat3{ {x1, y1, 1}, {0, 0, 0}, {0, 0, 0} };
		glm::mat3 pos2 = IR * glm::mat3{ {x2, y2, 1}, {0, 0, 0}, {0, 0, 0} };
		glm::mat3 pos3 = IR * glm::mat3{ {x3, y3, 1}, {0, 0, 0}, {0, 0, 0} };
		double height = static_cast<double>(pos3[0][1]) - static_cast<double>(pos1[0][1]);
		double width = static_cast<double>(pos2[0][0]) - static_cast<double>(pos1[0][0]);
		double shear = static_cast<double>(pos3[0][0]) - static_cast<double>(pos1[0][0]);
		glm::mat3 S = { {width, 0, 0}, {shear, height, 0}, {0, 0, 1} };
		glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x1, y1, 1} };
		OpenGLPolygon.globject.mdl_xform = T * R * S;
		OpenGLPolygon.globject.xform = OpenGLPolygon.glsetting.translatemat * OpenGLPolygon.glsetting.rotatemat * OpenGLPolygon.glsetting.scalemat;
		OpenGLPolygon.globject.color = { OpenGLPolygon.glsetting.FR, OpenGLPolygon.glsetting.FG, OpenGLPolygon.glsetting.FB, OpenGLPolygon.glsetting.FA };
		OpenGLPolygon.globject.mdl_ref = "triangle";
		OpenGLPolygon.globject.draw();
	}
	if (OpenGLPolygon.glsetting.noline == GL_FALSE) {
		draw_line(x1, y1, x2, y2);
		draw_line(x2, y2, x3, y3);
		draw_line(x3, y3, x1, y1);
	}
}

void draw_rect(double x1, double y1, double width, double height) {
	glm::vec2 pos1{ 0, 0 };
	glm::vec2 pos2{ 0, 0 };
	glm::vec2 pos3{ 0, 0 };
	glm::vec2 pos4{ 0, 0 };
	pos1 = { x1, y1 };
	pos2 = { x1 + width, y1 };
	pos3 = { x1 + width, y1 + height };
	pos4 = { x1, y1 + height };
	if (OpenGLPolygon.glsetting.nofill == GL_FALSE) {
		if (OpenGLPolygon.glsetting.noline == GL_FALSE) {
			no_outline();
			draw_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y);
			draw_triangle(pos1.x, pos1.y, pos3.x, pos3.y, pos4.x, pos4.y);
			yes_outline();
		}
		else {
			draw_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y);
			draw_triangle(pos1.x, pos1.y, pos3.x, pos3.y, pos4.x, pos4.y);
		}
	}
	if (OpenGLPolygon.glsetting.noline == GL_FALSE) {
		draw_line(pos1.x, pos1.y, pos2.x, pos2.y);
		draw_line(pos2.x, pos2.y, pos3.x, pos3.y);
		draw_line(pos3.x, pos3.y, pos4.x, pos4.y);
		draw_line(pos4.x, pos4.y, pos1.x, pos1.y);
	}
}

void draw_ellipse(double x, double y, double width, double height) {
	if (OpenGLPolygon.glsetting.nofill == GL_FALSE) {
		glm::mat3 S = { {width / 2, 0, 0}, {0, height / 2, 0}, {0, 0, 1} };
		glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x, y, 1} };
		OpenGLPolygon.globject.mdl_xform = T * S;
		OpenGLPolygon.globject.xform = OpenGLPolygon.glsetting.translatemat * OpenGLPolygon.glsetting.rotatemat * OpenGLPolygon.glsetting.scalemat;
		OpenGLPolygon.globject.color = { OpenGLPolygon.glsetting.FR, OpenGLPolygon.glsetting.FG, OpenGLPolygon.glsetting.FB, OpenGLPolygon.glsetting.FA };
		OpenGLPolygon.globject.mdl_ref = "ellipse";
		OpenGLPolygon.globject.draw();
	}
	if (OpenGLPolygon.glsetting.noline == GL_FALSE) {
		double slice = PI / 25;
		for (int i = 0; i < 50; ++i) {
			int j = i + 1;
			double x1 = x + width / 2 * cos(slice * i);
			double y1 = y + height / 2 * sin(slice * i);
			double x2 = x + width / 2 * cos(slice * j);
			double y2 = y + height / 2 * sin(slice * j);
			draw_line(x1, y1, x2, y2);
		}
	}
}

void Polygon::GLSetting::clear() {
	LR = 0.f;
	LG = 0.f;
	LB = 0.f;
	LA = 1.f;
	FR = 0.f;
	FG = 0.f;
	FB = 0.f;
	FA = 1.f;
	LW = 1.f;
	noline = GL_FALSE;
	nofill = GL_FALSE;
	scalemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	rotatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	translatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
}

void push_settings() {
	Polygon::GLSetting tempsetting;
	tempsetting = OpenGLPolygon.glsetting;
	OpenGLPolygon.tempsettings.push_back(tempsetting);
	OpenGLPolygon.glsetting.clear();
}

void pop_settings() {
	OpenGLPolygon.glsetting = OpenGLPolygon.tempsettings.back();
	OpenGLPolygon.tempsettings.pop_back();
}

void apply_scale(double x1, double y1, double x2, double y2) {
	OpenGLPolygon.glsetting.scalemat = glm::mat3{ {x1, 0, 0}, {0, y1, 0}, {x2, y2, 1} }
	* glm::mat3{ {1, 0, 0}, {0, 1, 0}, { -x2, -y2, 1} } * OpenGLPolygon.glsetting.scalemat;
}

void apply_rotate(double angle, double x, double y) {
	OpenGLPolygon.glsetting.rotatemat = glm::mat3{ {cos(angle), sin(angle), 0}, { -sin(angle), cos(angle), 0}, {x, y, 1} }
	*glm::mat3{ {1, 0, 0}, {0, 1, 0}, { -x, -y, 1} } *OpenGLPolygon.glsetting.rotatemat;
}

void apply_translate(double x, double y) {
	OpenGLPolygon.glsetting.translatemat = glm::mat3{ {1, 0, 0}, {0, 1, 0}, {x, y, 1} } * OpenGLPolygon.glsetting.translatemat;
}