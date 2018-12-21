#include "PositionHeatmap.h"

#include <algorithm> 

PositionHeatmap::PositionHeatmap(std::vector<glm::vec3> positions, std::vector<glm::vec3> colours)
{
	m_positions = positions;
	m_colours = colours;

	m_drawCount = positions.size();

	m_shader = new Shader("shaders/positionheatmap");
	GLint vertexAttrib = glGetAttribLocation(m_shader->id(), "vertex");
	GLint colourAttrib = glGetAttribLocation(m_shader->id(), "colour");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Buffer point data
	glGenBuffers(1, &m_pointBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pointBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexAttrib);

	// Buffer colour data
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colours.size() * sizeof(glm::vec3), &m_colours[0], GL_STATIC_DRAW);
	glVertexAttribPointer(colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colourAttrib);
}

PositionHeatmap::~PositionHeatmap()
{
	delete m_shader;
	glDeleteBuffers(1, &m_pointBuffer);
	glDeleteBuffers(1, &m_colourBuffer);
	glDeleteVertexArrays(1, &m_vao);
}

void PositionHeatmap::draw(Camera &camera, float zoom)
{
	float pointSize = 12.0f * zoom;

	m_shader->use();

	GLuint mvpAttrib = glGetUniformLocation(m_shader->id(), "mvp");
	glUniformMatrix4fv(mvpAttrib, 1, false, &camera.getMVP()[0][0]);

	GLint pointUniform = glGetUniformLocation(m_shader->id(), "size");
	glUniform1f(pointUniform, pointSize);

	glBindVertexArray(m_vao);

	glDrawArrays(GL_POINTS, 0, m_drawCount);
}