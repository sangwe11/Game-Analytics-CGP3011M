#include "Font.h"

#include <iostream>

Font::Font(FT_Library &ft, const std::string &font)
{
	m_shader = new Shader("shaders/font");
	GLint vertexAttrib = glGetAttribLocation(m_shader->id(), "vertex");
	colourUniform = glGetUniformLocation(m_shader->id(), "colour");
	fontUniform = glGetUniformLocation(m_shader->id(), "font");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_glyphBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glyphBuffer);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

	FT_New_Face(ft, font.c_str(), 0, &m_face);
}

void Font::draw(const char *text, float x, float y, float sx, float sy, unsigned int size, glm::vec4 colour)
{
	const char *p;
	GLuint glGlyph;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &glGlyph);
	glBindTexture(GL_TEXTURE_2D, glGlyph);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindBuffer(GL_ARRAY_BUFFER, m_glyphBuffer);
	glBindVertexArray(m_vao);

	m_shader->use();

	glUniform4fv(colourUniform, 4, &colour[0]);
	glUniform1i(fontUniform, 0);

	FT_Set_Pixel_Sizes(m_face, 0, size);

	for (p = text; *p; p++)
	{
		if(FT_Load_Char(m_face, *p, FT_LOAD_RENDER) != 0)
			continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
		
		float x2 = x + m_face->glyph->bitmap_left * sx;
		float y2 = -y - m_face->glyph->bitmap_top * sy;
		float w = m_face->glyph->bitmap.width * sx;
		float h = m_face->glyph->bitmap.rows  * sy;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2, 1, 0 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		// Buffer data
		glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(GL_FLOAT), box);

		// Draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (m_face->glyph->advance.x >> 6) * sx;
		y += (m_face->glyph->advance.y >> 6) * sy;	
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteTextures(1, &glGlyph);
}