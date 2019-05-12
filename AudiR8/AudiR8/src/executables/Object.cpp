#include "Object.h"

Object::Object(Mesh Geometry, GLuint ShaderID, Material Material)
{
	m_Geometry = Geometry;
	m_Material = Material;
	m_ShaderID = ShaderID;
	m_modelMatrixID= glGetUniformLocation(m_ShaderID, "modelMatrix");
}

Object::~Object()
{
}

void Object::setModelMatrix(glm::mat4 x)
{
	m_modelMatrix = x;
}

void Object::render()
{
	glUniformMatrix4fv(m_modelMatrixID, 1, false, glm::value_ptr(m_modelMatrix));
	m_Material.render();
	m_Geometry.bindMesh();
	glDrawArrays(GL_TRIANGLES, 0, m_Geometry.size);
	m_Geometry.unbindMesh();
}

