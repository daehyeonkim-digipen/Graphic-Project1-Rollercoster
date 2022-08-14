#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
class ShaderList
{
	enum ShaderType {
		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
	};
public:
	~ShaderList();
	GLuint get_handle(unsigned int input);
	void add_new_handle(std::string vert_file_name, std::string frag_file_name, std::string geo_file_name = "", std::string tesc_file_name = "", std::string tese_file_name = "");
private:
	void add_shader(GLuint, std::string, ShaderType);
	GLboolean link_shader(GLuint);
	GLboolean valid_shader(GLuint);

private:
	std::vector<GLuint> Handles;
	std::string shader_log;

};
