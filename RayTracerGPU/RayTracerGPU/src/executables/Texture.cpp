#include "Texture.h"
#include "../Tools/stb/stb_image.h"

Texture::~Texture()
{
}

void Texture::unbind2D()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int bytesPerPixel)
{
	m_width = width;
	m_height = height;
	m_bytesPerPixel = bytesPerPixel;
	unsigned int valuesSize = m_width * m_height * m_bytesPerPixel;
	std::vector<unsigned char> values(valuesSize, 255);
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

Texture::Texture(std::string filename)
{
	filename = TEXTURE_PATH + filename;
	unsigned char* data = stbi_load(filename.c_str(), &m_width, &m_height, &m_bytesPerPixel, 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (m_bytesPerPixel < 3) {
		std::cout << "ERROR: Unable to open image" << filename << std::endl;
		return;
	}
	else if (m_bytesPerPixel == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (m_bytesPerPixel == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	std::cout << "SUCCESS: image loaded from " << filename << std::endl;
}

GLuint Texture::getTextureID() {
	return m_texture;
}

void Texture::bind2D()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

unsigned int Texture::getWidth() {
	return m_width;
}

unsigned int Texture::getHeight() {
	return m_height;
}