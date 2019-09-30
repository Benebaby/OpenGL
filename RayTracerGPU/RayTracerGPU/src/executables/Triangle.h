#pragma once
#include <glm/glm.hpp>

class Triangle
{
private:
	glm::vec3 m_A;
	float pad;
	glm::vec3 m_u;
	float pad1;
	glm::vec3 m_v;
	float pad2;
	glm::vec3 m_normal;
	int m_matID;
public:
	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, int matID);
};