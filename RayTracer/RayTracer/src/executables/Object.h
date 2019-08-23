#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Object
{
public:
	virtual float intersect(Ray ray, float max) = 0;
	virtual glm::vec3 getNormal(Ray ray, float distance) = 0;
	virtual glm::vec3 getColor() = 0;
};