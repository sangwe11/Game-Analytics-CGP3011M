#include "Camera.h"

#include <GLM/ext.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 rotation, const float &fov, const float &zNear, const float &zFar, const int &displayX, const int &displayY)
{
	m_position = position;
	m_rotation = rotation;
	m_fov = fov;
	m_zNear = zNear;
	m_zFar = zFar;

	m_projection = glm::perspective(glm::radians(fov), (float)displayX / (float)displayY, zNear, zFar);
}

const glm::mat4 Camera::getView()
{
	glm::mat4 positionMatrix = glm::translate(m_position);
	glm::mat4 rotationMatrix = glm::eulerAngleZ(m_rotation.z) * glm::eulerAngleY(m_rotation.y) * glm::eulerAngleX(m_rotation.x);

	return glm::inverse(positionMatrix * rotationMatrix);
}
