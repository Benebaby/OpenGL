#include "Camera.h"

Camera::Camera(int width, int height, float fov)
{
	m_fov = fov;
	m_width = width;
	m_height = height;
	radius = 5.0f;
	Angle = glm::vec2(0.0f, Pi / 2);
	Oldpos = glm::vec2(m_width / 2, m_height / 2);
}


Camera::~Camera()
{
}

glm::vec3 Camera::getEye()
{
	return eye;
}

float Camera::getFov()
{
	return m_fov;
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

void Camera::updateCamPos(double xpos, double ypos, bool active) {
	if (active) {
		Newpos = glm::vec2(xpos, ypos);
		float xAngle = (Newpos.x - Oldpos.x) / m_width * 2 * Pi;
		float yAngle = (Newpos.y - Oldpos.y) / m_height * Pi;
		Angle -= glm::vec2(xAngle, yAngle);
		Angle.y = glm::max(Angle.y, 0.000001f);
		Angle.y = glm::min(Angle.y, Pi);
		Oldpos = glm::vec2(xpos, ypos);
	}
	else {
		Oldpos = glm::vec2(xpos, ypos);
	}

	eye.x = radius * sin(Angle.y) * sin(Angle.x);
	eye.y = radius * cos(Angle.y);
	eye.z = radius * sin(Angle.y) * cos(Angle.x);
	m_viewMatrix = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::updateRadius(double yoffset) {
	radius -= (float)(yoffset * 0.1);
	radius = glm::max(radius, 0.000001f);

	eye.x = radius * sin(Angle.y) * sin(Angle.x);
	eye.y = radius * cos(Angle.y);
	eye.z = radius * sin(Angle.y) * cos(Angle.x);
	m_viewMatrix = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setResolution(unsigned int w, unsigned int h) {
	m_width = w;
	m_height = h;
}
