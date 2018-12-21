#ifndef _POSITIONHEATMAP_H
#define _POSITIONHEATMAP_H

#include <vector>
#include <GLM\glm.hpp>
#include <GL\glew.h>
#include "Shader.h"
#include "Camera.h"

class PositionHeatmap
{
public:
	PositionHeatmap(std::vector<glm::vec3> positions, std::vector<glm::vec3> colours);
	~PositionHeatmap();

	int getFrames() { return m_drawCount; }

	void draw(Camera &camera, float zoom);

private:
	GLuint m_vao;
	GLuint m_pointBuffer;
	GLuint m_colourBuffer;

	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_colours;

	Shader *m_shader;

	int m_drawCount;
};

#endif