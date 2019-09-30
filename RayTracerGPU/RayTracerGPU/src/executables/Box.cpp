#include "Box.h"

Box::Box(glm::vec3 A, glm::vec3 B, int matID)
{
	m_matID = matID;
	m_A = A;
	m_B = B;
	pad0 = 0;
}
