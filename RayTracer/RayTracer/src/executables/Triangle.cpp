//#include "Triangle.h"
//
//float Triangle::intersect(Ray ray, float max)
//{
//	glm::vec3 dir = ray.m_dir;
//	glm::vec3 d_v = glm::cross(dir, m_v);
//	float det = glm::dot(d_v, m_u);
//	if (abs(det) < 0.0001)
//		return -1.0f;
//	float invDet = 1.f / det;
//	glm::vec3 P = ray.m_pos;
//	glm::vec3 w = P - m_A;
//	float s = glm::dot(d_v, w) * invDet;
//	if (s < 0.f || s > 1.f)
//		return -1.0f;
//	glm::vec3 w_u = glm::cross(w, m_u);
//	float t = glm::dot(w_u, dir) * invDet;
//	if (t < 0.f || s + t > 1.f)
//		return -1.0f;
//	float l = glm::dot(w_u, m_v) * invDet;
//	if ((l < 0.f) || (l > max))
//		return -1.0f;
//	m_baryc = glm::vec3(1 - s - t, s, t);
//	return l;
//}
//
//glm::vec3 Triangle::getNormal(Ray ray, float distance)
//{
//
//}
//
//glm::vec3 Triangle::getColor() 
//{
//	return m_color;
//}