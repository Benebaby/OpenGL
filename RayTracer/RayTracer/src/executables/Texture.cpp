#include "Texture.h"

void Texture::createTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

Texture::~Texture()
{
	if (m_textureID != INVALID_GL_VALUE)
		glDeleteTextures(1, &m_textureID);
}

unsigned int Texture::getTexture() {
	return m_textureID;
}

Texture::Texture(int width, int height, int bytesPerPixel, unsigned char* data)
{
	m_height = height;
	m_width = width;
	m_bytesPerPixel = bytesPerPixel;
	m_data = data;
	m_textureID = INVALID_GL_VALUE;
	createTexture();
	setTexture(width, height, bytesPerPixel, data);
}

void Texture::setTexture(int width, int height, int bytesPerPixel, unsigned char* data)
{
	m_height = height;
	m_width = width;
	m_bytesPerPixel = bytesPerPixel;
	m_data = data;
	if (m_textureID == INVALID_GL_VALUE) 
		createTexture();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	if (m_bytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	}
	else if (m_bytesPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	}
	else
	{
		std::cout << "RESOLVED: Unknown format for bytes per pixel in texture, changed to 4" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
}

