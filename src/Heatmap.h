#ifndef _HEATMAP_H
#define _HEATMAP_H

#include <vector>
#include <GLM\glm.hpp>
#include <GL\glew.h>
#include "Shader.h"
#include "Camera.h"

class Heatmap
{
public:
	Heatmap(std::vector<glm::vec3> positions, glm::vec3 colour);
	~Heatmap();

	int getFrames() { return m_drawCount; }

	void draw(Camera &camera, int frames = -1);

private:
	GLuint m_vao;
	GLuint m_points;
	GLuint m_colours;

	std::vector<glm::vec3> m_positions;
	glm::vec3 m_colour;

	Shader *m_shader;

	int m_drawCount;
};

#endif