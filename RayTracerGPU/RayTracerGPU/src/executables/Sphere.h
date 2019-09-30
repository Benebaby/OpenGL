#pragma once
#include <glm/glm.hpp>

class Sphere
{
private:
	glm::vec3 m_position;
	int m_matID;
	glm::mat3x4 m_rotation;
	float m_radius;
	glm::vec3 pad;
public:
	Sphere(glm::vec3 position, float radius, glm::mat4 modelMatrix, int matID);
	Sphere(glm::vec3 position, float radius, int matID);
	void update(glm::mat4 modelMatrix);
};

