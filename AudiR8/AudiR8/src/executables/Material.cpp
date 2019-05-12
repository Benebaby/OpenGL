#include "Material.h"


Material::Material()
{
}

Material::Material(int Type, GLuint ShaderID)
{
	m_type = Type;
	shaderID = ShaderID;
	typeID = glGetUniformLocation(shaderID, "type");
}

Material::Material(int Type, GLuint ShaderID, glm::vec4 diffuse, glm::vec3 specular, glm::vec3 ambiente, float shininess)
{
	m_type = Type;
	shaderID = ShaderID;
	m_diffuseColor = diffuse;
	m_specularColor = specular;
	m_ambiente = ambiente;
	m_shininess = shininess;
	typeID = glGetUniformLocation(shaderID, "type");
	diffuseID = glGetUniformLocation(shaderID, "diffuse");
	specularID = glGetUniformLocation(shaderID, "specular");
	shininessID = glGetUniformLocation(shaderID, "shininess");
	ambienteID = glGetUniformLocation(shaderID, "ambiente");
}

Material::Material(int Type, GLuint ShaderID, Texture diffuseTexture, glm::vec3 specular, glm::vec3 ambiente, float shininess)
{
	m_type = Type;
	shaderID = ShaderID;
	m_diffuseTexture = diffuseTexture;
	m_specularColor = specular;
	m_ambiente = ambiente;
	m_shininess = shininess;
	typeID = glGetUniformLocation(shaderID, "type");
	tex1ID = glGetUniformLocation(shaderID, "basemap");
	specularID = glGetUniformLocation(shaderID, "specular");
	shininessID = glGetUniformLocation(shaderID, "shininess");
	ambienteID = glGetUniformLocation(shaderID, "ambiente");
}

Material::Material(int Type, GLuint ShaderID, Texture diffuseTexture, Texture roughnessTexture, Texture normalTexture, glm::vec3 ambiente, float shininess)
{
	m_type = Type;
	shaderID = ShaderID;
	m_diffuseTexture = diffuseTexture;
	m_normalTexture = normalTexture;
	m_roughnessTexture = roughnessTexture;
	m_ambiente = ambiente;
	m_shininess = shininess;
	typeID = glGetUniformLocation(shaderID, "type");
	tex1ID = glGetUniformLocation(shaderID, "basemap");
	tex2ID = glGetUniformLocation(shaderID, "normalmap");
	tex3ID = glGetUniformLocation(shaderID, "roughmap");
	shininessID = glGetUniformLocation(shaderID, "shininess");
	ambienteID = glGetUniformLocation(shaderID, "ambiente");
}

Material::Material(int Type, GLuint ShaderID, Texture normalTexture, glm::vec4 diffuse, glm::vec3 specular, glm::vec3 ambiente, float shininess)
{
}

Material::Material(int Type, GLuint ShaderID, Texture cubemapTexture, glm::vec4 diffuse, glm::vec3 specular, int metalness, glm::vec3 ambiente, float shininess)
{
	m_type = Type;
	shaderID = ShaderID;
	m_diffuseColor = diffuse;
	m_specularColor = specular;
	m_ambiente = ambiente;
	m_shininess = shininess;
	m_metalness = metalness;
	m_cubemapTexture = cubemapTexture;
	typeID = glGetUniformLocation(shaderID, "type");
	metalnessID = glGetUniformLocation(shaderID, "metalness");
	diffuseID = glGetUniformLocation(shaderID, "diffuse");
	specularID = glGetUniformLocation(shaderID, "specular");
	shininessID = glGetUniformLocation(shaderID, "shininess");
	ambienteID = glGetUniformLocation(shaderID, "ambiente");
	tex4ID = glGetUniformLocation(shaderID, "reflectionCube");
}

Material::Material(int Type, GLuint ShaderID, Texture cubemapTexture)
{
	m_type = Type;
	shaderID = ShaderID;
	m_cubemapTexture = cubemapTexture;
	typeID = glGetUniformLocation(shaderID, "type");
	tex4ID = glGetUniformLocation(shaderID, "skybox");
}

void Material::render()
{
	if (m_type == 0) {
		glUniform1i(typeID, m_type);
	}
	if (m_type == 1) {
		glUniform1i(typeID, m_type);
		glUniform4fv(diffuseID, 1, glm::value_ptr(m_diffuseColor));
		glUniform3fv(specularID, 1, glm::value_ptr(m_specularColor));
		glUniform3fv(ambienteID, 1, glm::value_ptr(m_ambiente));
		glUniform1f(shininessID, m_shininess);
	}

	if (m_type == 2) {
		glUniform1i(typeID, m_type);
		glUniform3fv(specularID, 1, glm::value_ptr(m_specularColor));
		glUniform3fv(ambienteID, 1, glm::value_ptr(m_ambiente));
		glUniform1f(shininessID, m_shininess);
		glUniform1i(tex1ID, 0);
		glActiveTexture(GL_TEXTURE0);
		m_diffuseTexture.bind2D();
	}

	if (m_type == 3) {
		glUniform1i(typeID, m_type);
		glUniform3fv(ambienteID, 1, glm::value_ptr(m_ambiente));
		glUniform1f(shininessID, m_shininess);
		glUniform1i(tex1ID, 0);
		glActiveTexture(GL_TEXTURE0);
		m_diffuseTexture.bind2D();
		glUniform1i(tex2ID, 1);
		glActiveTexture(GL_TEXTURE1);
		m_normalTexture.bind2D();
		glUniform1i(tex3ID, 2);
		glActiveTexture(GL_TEXTURE2);
		m_roughnessTexture.bind2D();
	}

	if (m_type == 5) {
		glUniform1i(typeID, m_type);
		glUniform3fv(ambienteID, 1, glm::value_ptr(m_ambiente));
		glUniform1f(shininessID, m_shininess);
		glUniform4fv(diffuseID, 1, glm::value_ptr(m_diffuseColor));
		glUniform3fv(specularID, 1, glm::value_ptr(m_specularColor));
		glUniform1i(metalnessID, m_metalness);
		glUniform3fv(ambienteID, 1, glm::value_ptr(m_ambiente));
		glUniform1i(tex4ID, 3);
		glActiveTexture(GL_TEXTURE3);
		m_cubemapTexture.bind3D();
	}

	if (m_type == 6) {
		glUniform1i(typeID, m_type);
		glUniform1i(tex4ID, 3);
		glActiveTexture(GL_TEXTURE3);
		m_cubemapTexture.bind3D();
	}
}

Material::~Material()
{
}
