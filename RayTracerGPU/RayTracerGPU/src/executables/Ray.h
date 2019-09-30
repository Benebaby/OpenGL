#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray();
	Ray(glm::vec3 pos, glm::vec3 dir);
	glm::vec3 m_pos, m_dir;
};

