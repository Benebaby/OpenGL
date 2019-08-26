#include "Ray.h"

Ray::Ray(glm::vec3 pos, glm::vec3 dir) {
	m_pos = pos;
	m_dir = dir;
}

Ray::Ray() {
}