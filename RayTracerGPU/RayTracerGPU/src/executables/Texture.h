#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

class Texture
{
private:
	GLuint m_texture;
	int m_width, m_height, m_bytesPerPixel;
public:
	Texture(unsigned int width, unsigned int height, unsigned int bytesPerPixel);
	Texture(std::string filename);
	unsigned int getWidth();
	unsigned int getHeight();
	GLuint getTextureID();
	void bind2D();
	~Texture();
	void unbind2D();
};

