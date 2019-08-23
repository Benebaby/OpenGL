#include "Sphere.h"

Sphere::Sphere(glm::vec3 position, float radius, glm::vec3 color)
{
	m_color = color;
	m_position = position;
	m_radius = radius;
}

float Sphere::intersect(Ray ray, float max) 
{
	glm::vec3 m = m_position - ray.m_pos;
	float b = glm::dot(m, ray.m_dir);
	float mr2 = glm::dot(m, m) - m_radius * m_radius;
	if ((mr2 >= 0) && (b <= 0))
		return -1.0f;
	float disc = b * b - mr2;
	if (disc < 0.0f)
		return -1.0f;
	disc = sqrtf(disc);
	float t;
	if (mr2 > 0)
		t = (b - disc);
	else
		t = (b + disc);
	if ((t < 0.001f) || (t > max))
		return -1.0f;
	return t;
}

glm::vec3 Sphere::getNormal(Ray ray, float distance) 
{
	glm::vec3 point = ray.m_pos + ray.m_dir * distance;
	return glm::normalize(point - m_position);
}

glm::vec3 Sphere::getColor() 
{
	return m_color;
}