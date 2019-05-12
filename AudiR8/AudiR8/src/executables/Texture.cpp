#include "Texture.h"

Texture::~Texture()
{
}

Texture::Texture() {

}

Texture::Texture(int width, int height) {
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

Texture::Texture(std::string filename)
{
	filename = TEXTURE_PATH + filename;
	int width, height, bytesPerPixel;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bytesPerPixel, 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (bytesPerPixel < 3) {
		std::cout << "ERROR: Unable to open image" << filename << std::endl;
		return;
	}
	else if (bytesPerPixel == 3) {
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

	stbi_image_free(data);

	std::cout << "SUCCESS: image loaded from " << filename << std::endl;
}

Texture::Texture(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
{
	std::vector<std::string> faces = { right, left, top, bottom, front, back };
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	std::string filename;

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		filename = TEXTURE_PATH + faces[i];
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);


		if (nrChannels < 3) {
			std::cout << "ERROR: Unable to open image" << filename << std::endl;
			return;
		}
		else if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	std::cout << "SUCCESS: cubemap loaded from " << TEXTURE_PATH << std::endl;
}

GLuint Texture::getTexture() {
	return m_texture;
}

void Texture::bind2D() {
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::bind3D() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}
