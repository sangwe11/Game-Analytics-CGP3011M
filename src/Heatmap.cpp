#include "Heatmap.h"

Heatmap::Heatmap(std::vector<glm::vec3> positions, glm::vec3 colour)
{
	m_positions = positions;
	m_colour = colour;
	m_drawCount = positions.size();

	std::vector<glm::vec3> colourList;

	for (unsigned int i = 0; i < positions.size(); ++i)
		colourList.push_back(m_colour);

	m_shader = new Shader("shaders/heatmap");
	GLint vertexAttrib = glGetAttribLocation(m_shader->id(), "vertex");
	GLint colourAttrib = glGetAttribLocation(m_shader->id(), "colour");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Buffer point data
	glGenBuffers(1, &m_points);
	glBindBuffer(GL_ARRAY_BUFFER, m_points);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexAttrib);

	// Buffer colour data
	glGenBuffers(1, &m_colours);
	glBindBuffer(GL_ARRAY_BUFFER, m_colours);
	glBufferData(GL_ARRAY_BUFFER, colourList.size() * sizeof(glm::vec3), &colourList[0], GL_STATIC_DRAW);
	glVertexAttribPointer(colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colourAttrib);
}

Heatmap::~Heatmap()
{
	delete m_shader;
	glDeleteBuffers(1, &m_points);
	glDeleteBuffers(1, &m_colours);
	glDeleteVertexArrays(1, &m_vao);
}

void Heatmap::draw(Camera &camera, int frames)
{
	m_shader->use();

	GLuint mvpAttrib = glGetUniformLocation(m_shader->id(), "mvp");
	glUniformMatrix4fv(mvpAttrib, 1, false, &camera.getMVP()[0][0]);

	glBindVertexArray(m_vao);

	if (frames <= 0)
		glDrawArrays(GL_POINTS, 0, m_drawCount);
	else
		glDrawArrays(GL_POINTS, 0, frames);
}