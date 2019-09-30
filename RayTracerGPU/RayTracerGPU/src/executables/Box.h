#pragma once
#include <glm/glm.hpp>
class Box
{
private:
	glm::vec3 m_A;
	int m_matID;
	glm::vec3 m_B;
	float pad0;
public:
	Box(glm::vec3 A, glm::vec3 B, int matID);
};