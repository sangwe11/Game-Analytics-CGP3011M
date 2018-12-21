#ifndef _FONT_H
#define _FONT_H

#include <GLM\glm.hpp>
#include <GL\glew.h>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

class Font
{
public:
	Font(FT_Library &ft, const std::string &font);
	void draw(const char *text, float x, float y, float sx, float sy, unsigned int size, glm::vec4 colour);

private:
	std::string m_font;
	GLuint m_vao;
	GLuint m_glyphBuffer;
	GLint colourUniform;
	GLint fontUniform;
	Shader *m_shader;
	FT_Face m_face;
};

#endif