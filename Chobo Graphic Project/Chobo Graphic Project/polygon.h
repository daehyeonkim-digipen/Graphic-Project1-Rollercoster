#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <string>
#include "math.h"

//width values from 1 to 10
void set_line_width(double width);

//RGBA values from 0 to 255
void set_line_color(double r, double g, double b, double a = 255);

//draw polygon without line
void no_outline();

//draw polygon with line
void yes_outline();

//draw polygon without fill
void no_fill();

//draw polygon with fill
void yes_fill();

//RGBA values from 0 to 255
void set_fill_color(double r, double g, double b, double a = 255);

//starting point to ending point
void draw_line(double x1, double y1, double x2, double y2);

//3 points to make triangle
void draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3);

//initial point and width, height
void draw_rect(double x1, double y1, double width, double height);

//center and radius
void draw_ellipse(double x, double y, double width, double height);

//save settings
void push_settings();

//get settings
void pop_settings();

//scale by x1, y1 respect to point x2, y2
void apply_scale(double x1, double y1, double x2, double y2);

//rotate by angle of degrees counter-clockwise respect to point x, y
void apply_rotate(double angle, double x, double y);

//translate by x, y
void apply_translate(double x, double y);

class Polygon {
private:
	struct Vertex {
		GLfloat x;
		GLfloat y;
	};

	struct GLModel {
		GLenum primitive_type;
		GLuint primitive_cnt;
		GLuint vaoid;
		GLuint draw_cnt;
	};

	void linesetup();
	void trisetup();
	void ellipsetup();

	struct GLObject {
		Vec4 color;
		Mat3 mdl_xform;
		Mat3 mdl_to_ndc_xform;
		Mat3 xform;
		std::string mdl_ref;
		GLfloat width = 0;

		void draw();
	};

public:
	Polygon(int windowW, int windowH) {
		NDC = { {2.f / windowW, 0, 0}, {0, 2.f / windowH, 0}, {-1.f, -1.f, 1.f} };
		linesetup();
		trisetup();
		ellipsetup();
	}
	Polygon() = default;

	std::map<std::string, GLModel> models;
	GLObject globject;
	Mat3 NDC = {};

	struct GLSetting {
		GLfloat LR = 0.f;
		GLfloat LG = 0.f;
		GLfloat LB = 0.f;
		GLfloat LA = 1.f;
		GLfloat FR = 0.f;
		GLfloat FG = 0.f;
		GLfloat FB = 0.f;
		GLfloat FA = 1.f;
		GLfloat LW = 1.f;
		GLboolean noline = GL_FALSE;
		GLboolean nofill = GL_FALSE;
		Mat3 scalemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
		Mat3 rotatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
		Mat3 translatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
		void clear();
	};

	GLSetting glsetting;
	std::vector<GLSetting> tempsettings;
};