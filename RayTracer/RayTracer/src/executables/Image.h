#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"

class Image
{
	public:
		Image(unsigned int w, unsigned int h);
		~Image();
		GLuint getID();
		void set(unsigned int x, unsigned int y, glm::vec3 c);
		void upload();
		unsigned int m_width, m_height;
	private:
		Texture* m_texture;
		std::vector<unsigned char> m_values;
};

