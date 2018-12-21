#ifndef _TRAJECTORY_H
#define _TRAJECTORY_H

#include <vector>
#include <GLM\glm.hpp>
#include <GL\glew.h>
#include "Shader.h"
#include "Camera.h"

class Trajectory
{
public:
	Trajectory(std::vector<glm::vec3> positions, std::vector<float> speed, glm::vec3 colour);
	~Trajectory();

	int getFrames() { return m_drawCount; }

	void draw(Camera &camera, bool speed, int frames = -1);

private:
	GLuint m_vao;
	GLuint m_points;
	GLuint m_colours;

	std::vector<glm::vec3> m_positions;
	std::vector<float> m_speed;
	glm::vec3 m_colour;

	Shader *m_shader;

	int m_drawCount;
};

#endif