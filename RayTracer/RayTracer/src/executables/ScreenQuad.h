#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ScreenQuad
{
private:
	GLfloat quadVertices[12] = {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
	GLfloat quadTexCoords[12] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
	
	GLuint VAO;
	GLuint VBO[2];
public:
	ScreenQuad();
	~ScreenQuad();
	void render();
};

