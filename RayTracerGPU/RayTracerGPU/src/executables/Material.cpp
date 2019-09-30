#include "Material.h"

Material::Material(glm::vec4 diffuse, glm::vec3 specular, float shininess)
{
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_textureIndex = -1;
	m_materialType = 0;
}

Material::Material(int textureIndex, glm::vec3 specular, float shininess)
{
	m_textureIndex = textureIndex;
	m_specular = specular;
	m_shininess = shininess;
	m_materialType = 1;
}
