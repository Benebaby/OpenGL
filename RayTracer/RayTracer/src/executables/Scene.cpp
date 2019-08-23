#include "Scene.h"

Scene::Scene() {

}

void Scene::addObject(Object* obj) 
{
	m_objects.push_back(obj);
}

bool Scene::anyIntersection(Intersection* h) 
{
	for (Object* s : m_objects)
	{
		float temp = s->intersect(h->m_ray, h->m_distance);
		if (temp >= 0.0f)
		{
			return true;
		}
	}
	return false;
}

bool Scene::closestIntersection(Intersection* h)
{
	float nearestHit = FLT_MAX;
	bool isHit = false;
	for (Object* s : m_objects)
	{

		float temp = s->intersect(h->m_ray, h->m_distance);
		if (temp >= 0.0f)
		{
			isHit = true;
			if (temp < nearestHit) {
				h->m_object = s;
				h->m_distance = temp;
				nearestHit = temp;
			}
		}
	}
	return isHit;
}