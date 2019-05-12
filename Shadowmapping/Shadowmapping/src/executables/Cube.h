#ifndef _CUBE_H
#define _CUBE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Cube
{
public:
	Cube();
	Cube(float size);
	~Cube();
	void render();
private:
	void create(float size);
	void computeTangents();
	float m_size;
	int m_points;
	int m_indices;
	std::vector<int> m_index;
	std::vector<glm::vec4> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<glm::vec2> m_uvs;

	GLuint m_vao;
	GLuint m_vertexbuffer;
	GLuint m_normalbuffer;
	GLuint m_uvbuffer;
	GLuint m_indexlist;
	GLuint m_tangentbuffer;
};

#endif

