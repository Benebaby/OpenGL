#pragma once
#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere(glm::vec3 position, float radius);
	glm::vec3 m_position;
	float m_radius;
};

