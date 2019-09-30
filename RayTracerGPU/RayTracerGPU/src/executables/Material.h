#pragma once
#include <glm/glm.hpp>

class Material
{
public:
	Material(glm::vec4 diffuse, glm::vec3 specular, float shininess);
	Material(int textureIndex, glm::vec3 specular, float shininess);
private:
	glm::vec4 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
	int m_textureIndex;
	int m_materialType;
	float pad0 = 0; float pad1 = 0;
};