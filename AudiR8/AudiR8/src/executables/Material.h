#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include "Texture.h"

class Material
{
private:
	GLuint typeID, shaderID, diffuseID, specularID, shininessID, ambienteID, roughnessID, metalnessID, tex1ID, tex2ID, tex3ID, tex4ID;
	Texture m_diffuseTexture, m_roughnessTexture, m_metalnessTexture, m_normalTexture, m_cubemapTexture;
	glm::vec4 m_diffuseColor = glm::vec4(0.0f);
	glm::vec3 m_specularColor = glm::vec3(0.0f);
	glm::vec3 m_ambiente = glm::vec3(0.0f);;
	float m_roughness = 0.0f;
	float m_shininess = 0.0f;
	int m_metalness = 0;
	int m_type;

public:
	Material();
	Material(int Type, GLuint ShaderID);
	Material(int Type, GLuint ShaderID, glm::vec4 diffuse, glm::vec3 specular, glm::vec3 ambiente, float shininess);
	Material(int Type, GLuint ShaderID, Texture diffuseTexture, glm::vec3 specular, glm::vec3 ambiente, float shininess);
	Material(int Type, GLuint ShaderID, Texture diffuseTexture, Texture roughnessTexture, Texture normalTexture, glm::vec3 ambiente, float shininess);
	Material(int Type, GLuint ShaderID, Texture normalTexture, glm::vec4 diffuse, glm::vec3 specular, glm::vec3 ambiente, float shininess);
	Material(int Type, GLuint ShaderID, Texture cubemapTexture, glm::vec4 diffuse, glm::vec3 specular, int metalness, glm::vec3 ambiente, float shininess);
	Material(int Type, GLuint ShaderID, Texture cubemapTexture);
	~Material();
	void render();
};

