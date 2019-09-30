#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, float radius, glm::mat4 modelMatrix, int matID)
{
	m_matID = matID;
	m_position = position;
	m_radius = radius;
	update(modelMatrix);
}

Sphere::Sphere(glm::vec3 position, float radius, int matID)
{
	m_matID = matID;
	m_position = position;
	m_radius = radius;
	m_rotation = glm::mat3(1.0f);
}

void Sphere::update(glm::mat4 modelMatrix)
{
	m_position.x += modelMatrix[0][3];
	m_position.y += modelMatrix[1][3];
	m_position.z += modelMatrix[2][3];
	m_rotation[0] = modelMatrix[0];
	m_rotation[1] = modelMatrix[1];
	m_rotation[2] = modelMatrix[2];
}
