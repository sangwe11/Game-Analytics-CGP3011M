#include "Trajectory.h"

Trajectory::Trajectory(std::vector<glm::vec3> positions, std::vector<float> speed, glm::vec3 colour)
{
	m_positions = positions;
	m_speed = speed;
	m_colour = colour;
	m_drawCount = positions.size();

	std::vector<glm::vec3> colourList;

	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		if (speed[i] <= 0)
		{
			colourList.push_back(glm::vec3(0.0f));
		}
		else
		{
			colourList.push_back(m_colour * speed[i]);
		}
	}

	m_shader = new Shader("shaders/trajectory");
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

Trajectory::~Trajectory()
{
	delete m_shader;
	glDeleteBuffers(1, &m_points);
	glDeleteBuffers(1, &m_colours);
	glDeleteVertexArrays(1, &m_vao);
}

void Trajectory::draw(Camera &camera, bool speed, int frames)
{
	m_shader->use();

	GLuint mvpAttrib = glGetUniformLocation(m_shader->id(), "mvp");
	GLuint speedUniform = glGetUniformLocation(m_shader->id(), "speed");
	GLuint colourUniform = glGetUniformLocation(m_shader->id(), "lineColour");

	glUniformMatrix4fv(mvpAttrib, 1, false, &camera.getMVP()[0][0]);
	glUniform1i(speedUniform, speed);
	glUniform3fv(colourUniform, 1, &m_colour[0]);

	glBindVertexArray(m_vao);

	if (frames <= 0)
		glDrawArrays(GL_LINE_STRIP, 0, m_drawCount);
	else
		glDrawArrays(GL_LINE_STRIP, 0, frames);
}