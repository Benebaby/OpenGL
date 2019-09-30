#include "Scene.h"

Scene::Scene(GLuint ProgramID) {
	m_texID1 = glGetUniformLocation(ProgramID, "texture1");
	m_texID2 = glGetUniformLocation(ProgramID, "texture2");
	glGenBuffers(1, &ssbos);
	glGenBuffers(1, &ssbot);
	glGenBuffers(1, &ssbob);
	glGenBuffers(1, &ssbom);
}

void Scene::addSphere(Sphere s) {
	spheres.push_back(s);
}

void Scene::addTriangle(Triangle t)
{
	triangles.push_back(t);
}

void Scene::addBox(Box b)
{
	boxes.push_back(b);
}

void Scene::addMaterial(Material m)
{
	materials.push_back(m);
}

void Scene::addTexture(std::string filename)
{
	Texture test = Texture(filename);
	m_textureIDs.push_back(test);
}

void Scene::updateSphere(int index, glm::mat4 modelMatrix)
{
	spheres[index].update(modelMatrix);
}

void Scene::render() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbos);
	glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbos);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbot);
	glBufferData(GL_SHADER_STORAGE_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbot);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbob);
	glBufferData(GL_SHADER_STORAGE_BUFFER, boxes.size() * sizeof(Box), boxes.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbob);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbom);
	glBufferData(GL_SHADER_STORAGE_BUFFER, materials.size() * sizeof(Material), materials.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbom);

	//glBindTextures(m_colorTextureID, m_textureIDs.size(), m_textureIDs.data());
	glUniform1i(m_texID1, 0);
	glActiveTexture(GL_TEXTURE0);
	m_textureIDs[0].bind2D();
	glUniform1i(m_texID2, 1);
	glActiveTexture(GL_TEXTURE1);
	m_textureIDs[1].bind2D();
}