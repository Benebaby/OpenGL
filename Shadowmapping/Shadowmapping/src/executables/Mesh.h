#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Mesh
{
	private:
		void readObj(std::string filename);
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_texCoords;
		std::vector<glm::vec3> m_tangents;
		GLuint m_vertexArrayHandle;
		GLuint m_vertexBufferHandle[4];

	public:
		int size;
		Mesh(std::string filename);
		Mesh();
		void render();
		~Mesh();
};

