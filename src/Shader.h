#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <GL\glew.h>

static GLuint createShader(const std::string &name, const GLenum &type);
static bool checkShaderError(GLuint shader, GLuint flag, const std::string & errorMessage);
static bool checkProgramError(GLuint program, GLuint flag, const std::string & errorMessage);

class Shader
{
public:
	Shader(const std::string &name);
	~Shader();

	GLuint id();
	void use();

private:
	std::string m_name;
	
	GLuint m_program;
	GLuint m_vertex;
	GLuint m_frag;
};

#endif