#include "Intersection.h"

Intersection::Intersection(Ray ray, float distance, Object* object)
{
	m_ray = ray;
	m_distance = distance;
	m_object = object;
}

glm::vec3 Intersection::getHitPosition()
{
	return glm::vec3(m_ray.m_pos + m_ray.m_dir * m_distance);
}