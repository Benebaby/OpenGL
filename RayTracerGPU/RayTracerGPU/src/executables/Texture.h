#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class Texture
{
private:
	GLuint m_texture;
	unsigned int m_width, m_height, m_bytesPerPixel;
public:
	Texture(unsigned int width, unsigned int height, unsigned int bytesPerPixel);
	unsigned int getWidth();
	unsigned int getHeight();
	~Texture();
	GLuint getTextureID();
	void bind2D();
};

