#pragma once
#include <vector>
#include "Object.h"
#include "Intersection.h"

class Scene
{
private:
	std::vector<Object*> m_objects;
public:
	Scene();
	void addObject(Object* obj);
	bool anyIntersection(Intersection* h);
	bool closestIntersection(Intersection* h);
};