#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../Tools/stb/stb_image.h"
#include <iostream>
#include <stdio.h>
#include <vector>

class Texture
{
private:
	GLuint m_texture;
public:
	Texture();
	Texture(int width, int height);
	Texture(std::string filename);
	Texture(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);
	~Texture();
	GLuint getTexture();
	void bind2D();
	void bind3D();
};

