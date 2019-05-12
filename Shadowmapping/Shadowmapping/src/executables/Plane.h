#ifndef _PLANE_H
#define _PLANE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>


class Plane
{
public:
	Plane();
	Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
	~Plane();
	void render();
private:
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

	void computeTangents();
	void create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
};

#endif

