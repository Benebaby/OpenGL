#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera(int width, int height);
	~Camera();
	void getView(glm::vec3* x, glm::vec3* y, glm::vec3* z, glm::vec3* pos) const;
	glm::vec3 getEye();
	void updateCamPos(double xpos, double ypos, bool active);
	void updateRadius(double yoffset);

	float Pi = 3.141592f;
	float radius = 5.0f;
	glm::vec2 Angle = glm::vec2(0.0f, Pi / 2);
	glm::vec2 Oldpos = glm::vec2(m_width / 2, m_height / 2);
	glm::vec2 Newpos = glm::vec2(m_width / 2, m_height / 2);
	int m_width;
	int m_height;
private:
	glm::mat4 m_viewMatrix;
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
};

