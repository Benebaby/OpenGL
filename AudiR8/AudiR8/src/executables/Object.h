#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include "Mesh.h"
#include "Material.h"

class Object
{
public:
	Object(Mesh Geometry, GLuint ShaderID, Material Material);
	virtual ~Object();
	void render();
	void setModelMatrix(glm::mat4 x);
private:
	GLuint m_ShaderID;
	GLuint m_modelMatrixID;

	Mesh m_Geometry;
	Material m_Material;
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);
};