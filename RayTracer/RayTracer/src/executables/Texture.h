#pragma once
#define INVALID_GL_VALUE 0xFFFFFFFF
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
private:
	unsigned int m_textureID;
	int m_width, m_height;
	unsigned char* m_data = nullptr;
	int m_bytesPerPixel;
	void createTexture();
public:
	Texture(int width, int height, int bytesPerPixel, unsigned char* data);
	~Texture();
	void setTexture(int width, int height, int bytesPerPixel, unsigned char* data);
	unsigned int getTexture();
};

