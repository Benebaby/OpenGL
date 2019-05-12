#ifndef _SPHERE_H
#define _SPHERE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <glm/ext.hpp>

class Sphere
{
public:
	Sphere();
	Sphere( float radius);
	Sphere( glm::vec3 center, float radius);
	Sphere( float radius, int resolution);
	Sphere( glm::vec3 center, float radius, int resolution);
	~Sphere();

	glm::vec3 *get_center();
	void render();
	float get_radius();

private:
	void create( glm::vec3 center, float radius, int resolution);
	void computeTangents();

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
	glm::vec3 m_Center;
	float m_radius;
	int m_resolution;
};

#endif

