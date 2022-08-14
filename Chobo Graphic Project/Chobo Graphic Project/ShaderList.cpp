#include "ShaderList.h"
#include <sstream>
#include <fstream>
#include <iostream>

void ShaderList::add_new_handle(std::string vert_file_name, std::string frag_file_name, std::string geo_file_name, std::string tesc_file_name, std::string tese_file_name)
{
	GLuint pgm_handle = 0;
	if (pgm_handle <= 0) {
		pgm_handle = glCreateProgram();
		if (0 == pgm_handle) {
			throw std::runtime_error("Cannot create program handle");
		}
	}
	add_shader(pgm_handle, vert_file_name, VERTEX_SHADER);
	add_shader(pgm_handle, frag_file_name, FRAGMENT_SHADER);
	if(!geo_file_name.empty())
	{
		add_shader(pgm_handle, geo_file_name, GEOMETRY_SHADER);
	}
	if (!tesc_file_name.empty())
	{
		add_shader(pgm_handle, tesc_file_name, TESS_CONTROL_SHADER);
	}
	if (!tese_file_name.empty())
	{
		add_shader(pgm_handle, tese_file_name, TESS_EVALUATION_SHADER);
	}
	if (link_shader(pgm_handle) == GL_FALSE) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shader_log << "\n";
		std::exit(EXIT_FAILURE);
	}
	if (valid_shader(pgm_handle) == GL_FALSE) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shader_log << "\n";
		std::exit(EXIT_FAILURE);
	}
	Handles.push_back(pgm_handle);
}

void ShaderList::add_shader(GLuint pgm_handle, std::string file_name, ShaderType shader_type)
{
	std::ifstream infile(file_name);
	if (GL_FALSE == infile.good()) {
		throw std::runtime_error("Shader file not found");
	}
	std::ifstream shader_file(file_name, std::ifstream::in);
	if (!shader_file) {
		throw std::runtime_error("Error opening file " + file_name);
	}
	std::stringstream buffer;
	buffer << shader_file.rdbuf();
	shader_file.close();

	GLuint shader_handle = 0;
	switch (shader_type) {
	case VERTEX_SHADER: shader_handle = glCreateShader(GL_VERTEX_SHADER); break;
	case FRAGMENT_SHADER: shader_handle = glCreateShader(GL_FRAGMENT_SHADER); break;
	case GEOMETRY_SHADER: shader_handle = glCreateShader(GL_GEOMETRY_SHADER); break;
	case TESS_CONTROL_SHADER: shader_handle = glCreateShader(GL_TESS_CONTROL_SHADER); break;
	case TESS_EVALUATION_SHADER: shader_handle = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
	default:
		throw std::runtime_error("Incorrect shader type");
		break;
	}
	// load shader source code into shader object
	std::string buf = buffer.str();
	GLchar const* shader_code[] = { buf.c_str() };
	glShaderSource(shader_handle, 1, shader_code, NULL);

	// compile the shader
	glCompileShader(shader_handle);

	// check compilation status
	GLint comp_result;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &comp_result);
	if (GL_FALSE == comp_result)
	{
		shader_log = "Vertex shader compilation failed\n";
		GLint log_len;
		glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0) {
			GLchar* log = new GLchar[log_len];
			GLsizei written_log_len;
			glGetShaderInfoLog(shader_handle, log_len, &written_log_len, log);
			shader_log += log;
			std::cout << shader_log;
			delete[] log;
		}
	}
	else
	{
		glAttachShader(pgm_handle, shader_handle);
	}
	glDeleteShader(shader_handle);
}

GLboolean ShaderList::link_shader(GLuint pgm_handle)
{
	glLinkProgram(pgm_handle); // link the various compiled shaders

	// verify the link status
	GLint lnk_status;
	glGetProgramiv(pgm_handle, GL_LINK_STATUS, &lnk_status);
	if (GL_FALSE == lnk_status) {
		shader_log = "Failed to link shader program\n";
		GLint log_len;
		glGetProgramiv(pgm_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0) {
			GLchar* log_str = new GLchar[log_len];
			GLsizei written_log_len;
			glGetProgramInfoLog(pgm_handle, log_len, &written_log_len, log_str);
			shader_log += log_str;
			delete[] log_str;
		}
		return GL_FALSE;
	}
	return GL_TRUE;
}

GLboolean ShaderList::valid_shader(GLuint pgm_handle)
{
	glValidateProgram(pgm_handle);
	GLint status;
	glGetProgramiv(pgm_handle, GL_VALIDATE_STATUS, &status);
	if (GL_FALSE == status) {
		shader_log = "Failed to validate shader program for current OpenGL context\n";
		GLint log_len;
		glGetProgramiv(pgm_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0) {
			GLchar* log_str = new GLchar[log_len];
			GLsizei written_log_len;
			glGetProgramInfoLog(pgm_handle, log_len, &written_log_len, log_str);
			shader_log += log_str;
			delete[] log_str;
		}
		return GL_FALSE;
	}
	return GL_TRUE;
}

ShaderList::~ShaderList()
{
	for(auto& h : Handles)
	{
		glDeleteProgram(h);
	}
}

GLuint ShaderList::get_handle(unsigned int input)
{
	if(input >= Handles.size())
	{
		std::cout << "Wrong Handle index\n";
		std::cout << shader_log << "\n";
		std::exit(EXIT_FAILURE);
	}
	return Handles[input];
}
