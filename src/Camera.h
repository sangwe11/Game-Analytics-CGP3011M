#ifndef _CAMERA_H
#define _CAMERA_H

#include <GLM\glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 rotation, const float &fov, const float &zNear, const float &zFar, const int &displayX, const int &displayY);

	const glm::mat4 getMVP() { return m_projection * getView(); }
	const glm::mat4 getView();
	const glm::mat4 &getProjection() const { return m_projection; }
	const float &getFov() const { return m_fov; }
	const float &getZNear() const { return m_zNear; }
	const float &getZFar() const { return m_zFar; }

	void setPosition(glm::vec3 position) { m_position = position; }
	const glm::vec3 &getPosition() { return m_position; }

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;

	float m_fov;
	float m_zNear;
	float m_zFar;
	glm::mat4 m_projection;
};

#endif