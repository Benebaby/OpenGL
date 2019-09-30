#include "Triangle.h"

Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, int matID)
{
	m_matID = matID;
	m_A = A;
	m_u = B - A;
	m_v = C - A;
	m_normal = glm::cross(m_u, m_v);
	pad = 0; pad1 = 0; pad2 = 0;
}