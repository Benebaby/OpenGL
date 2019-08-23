#include "Camera.h"

Camera::Camera(int width, int height)
{
	m_width = width;
	m_height = height;
	radius = 5.0f;
	Angle = glm::vec2(0.0f, Pi / 2);
	Oldpos = glm::vec2(m_width / 2, m_height / 2);
}


Camera::~Camera()
{
}

void Camera::update() {
	eye.x = radius * sin(Angle.y) * sin(Angle.x);
	eye.y = radius * cos(Angle.y);
	eye.z = radius * sin(Angle.y) * cos(Angle.x);
	m_viewMatrix = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
}

glm::vec3 Camera::getEye()
{
	return eye;
}

void Camera::getView(glm::vec3* x, glm::vec3* y, glm::vec3* z, glm::vec3* pos) const
{
	*x = glm::vec3(glm::row(m_viewMatrix, 0));
	*y = glm::vec3(glm::row(m_viewMatrix, 1));
	*z = glm::vec3(glm::row(m_viewMatrix, 2));
	*pos = glm::vec3(glm::column(m_viewMatrix, 3));
	glm::mat3 mat_inv = glm::inverse(glm::mat3(m_viewMatrix));
	*pos = -mat_inv * *pos;
}
