#include "Texture.h"

Texture::~Texture()
{
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int bytesPerPixel)
{
	m_width = width;
	m_height = height;
	m_bytesPerPixel = bytesPerPixel;
	std::vector<unsigned char> values(m_width * m_height * m_bytesPerPixel, 255);
	unsigned char* data = values.data();

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (bytesPerPixel == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (bytesPerPixel == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureID() {
	return m_texture;
}

void Texture::bind2D() {
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

unsigned int Texture::getWidth() {
	return m_width;
}

unsigned int Texture::getHeight() {
	return m_height;
}