#include "Image.h"

Image::Image(unsigned int w, unsigned int h)
{
	m_width = w;
	m_height = h;
	m_texture = new Texture(w, h, 4, 0);
	m_values.resize(w * h * 4, 255);
	upload();
}

Image::~Image()
{
	delete m_texture;
}

void Image::set(unsigned int x, unsigned int y, glm::vec3 c) {
	unsigned int pixel = (y * m_width + x) * 4;
	for (unsigned int i = 0; i < 3; i++)
		m_values[pixel + i] = glm::clamp(c[i], 0.0f, 1.0f) * 255.0f;
}

void Image::upload() {
	m_texture->setTexture(m_width, m_height, 4, m_values.data());
}

GLuint Image::getID() {
	return m_texture->getTexture();
}
