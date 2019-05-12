#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera(int width, int height);
	~Camera();
	glm::mat4 getViewMatrix();
	void update();

	float Pi = 3.141592f;
	float radius;
	glm::vec2 Angle;
	glm::vec2 Oldpos;
	glm::vec2 Newpos;
	int m_width;
	int m_height;
	glm::vec3 eye;

private:
	glm::mat4 m_viewMatrix;
	

};

