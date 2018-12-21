#include "Shader.h"
#include "FileIO.h"

#include <iostream>

static GLuint createShader(const std::string &name, const GLenum &type)
{
	std::string ext;

	switch (type)
	{
	case GL_VERTEX_SHADER:
		ext = ".vert";
		break;
	case GL_FRAGMENT_SHADER:
		ext = ".frag";
		break;
	case GL_GEOMETRY_SHADER:
		ext = ".geom";
		break;
	default:
		ext = "";
		break;
	}

	// Create a new shader of shaderType
	GLuint shader = glCreateShader(type);

	std::string shaderSource = readFile(name + ext);

	// If shader source is empty, then it couldn't be loaded.
	if (shaderSource == "")
		return NULL;

	// Convert source to GLchar *
	const GLchar * source = shaderSource.c_str();
	GLint sourceLength = shaderSource.length();

	// Pass source to OpenGL and compile shader
	glShaderSource(shader, 1, &source, &sourceLength);
	glCompileShader(shader);

	// Check shader compile status
	if (!checkShaderError(shader, GL_COMPILE_STATUS, "Shader compile failed!"))
		std::cout << "Shader compiled successfully: " << name << ext << std::endl;

	return shader;
}

static bool checkShaderError(GLuint shader, GLuint flag, const std::string & errorMessage)
{
	GLint status;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shader, flag, &status);

	if (status == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cout << errorMessage + ": '" << error << "'" << std::endl;
		return true;
	}

	return false;

}

static bool checkProgramError(GLuint program, GLuint flag, const std::string & errorMessage)
{
	GLint status;
	GLchar error[1024] = { 0 };

	glGetProgramiv(program, flag, &status);

	if (status == GL_FALSE)
	{
		glGetProgramInfoLog(program, sizeof(error), NULL, error);
		std::cout << errorMessage + ": '" << error << "'" << std::endl;
		return true;
	}

	return false;

}

Shader::Shader(const std::string &name)
{
	m_name = name;
	m_program = glCreateProgram();

	m_vertex = createShader(name, GL_VERTEX_SHADER);
	m_frag = createShader(name, GL_FRAGMENT_SHADER);

	// Vertex shader is required!
	if (m_vertex != NULL)
		glAttachShader(m_program, m_vertex);
	else
	{
		std::cout << "Vertex shader is required!" << std::endl;
		exit(1);
	}

	// Fragment shader is required!
	if (m_frag != NULL)
		glAttachShader(m_program, m_frag);
	else
	{
		std::cout << "Fragment shader is required!" << std::endl;
		exit(1);
	}

	// Link the shader program
	glLinkProgram(m_program);

	// Check link status
	if (!checkProgramError(m_program, GL_LINK_STATUS, "Shader not linked correctly!"))
		std::cout << "Shader linked correctly!" <<std::endl;

	// Validate the program
	glValidateProgram(m_program);

	// Check program is valid
	if (!checkProgramError(m_program, GL_VALIDATE_STATUS, "Shader not valid!"))
	{
		std::cout << "Shader valid!" << std::endl;
	}

	// Bind the frag shader output variable
	glUseProgram(m_program);
	glBindFragDataLocation(m_program, 0, "finalColor");
}

Shader::~Shader()
{
	glDetachShader(m_program, m_vertex);
	glDeleteShader(m_vertex);
	glDetachShader(m_program, m_vertex);
	glDeleteShader(m_frag);
	glDeleteProgram(m_program);
}

GLuint Shader::id()
{
	return m_program;
}

void Shader::use()
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	if (id != m_program)
		glUseProgram(m_program);
}