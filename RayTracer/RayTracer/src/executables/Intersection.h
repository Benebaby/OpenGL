#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Object.h"

class Intersection
{
public:
	Ray m_ray;
	float m_distance;
	Object* m_object;
	glm::vec3 m_baryc;
	bool enter;
	Intersection(Ray ray, float distance, Object* object);
	glm::vec3 getHitPosition();
};

