#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Object.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 position, float radius, glm::vec3 color);
	float intersect(Ray ray, float max);
	glm::vec3 getNormal(Ray ray, float distance);
	glm::vec3 getColor();
	float m_radius;
	glm::vec3 m_position;
	glm::vec3 m_color;
};

