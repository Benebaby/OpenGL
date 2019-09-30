#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Triangle.h"
#include "Box.h"
#include "Sphere.h"
#include "Material.h"
#include "Texture.h"

class Scene
{
private:
	GLuint ssbot, ssbob, ssbos, ssbom, m_texID1, m_texID2;
	std::vector<Triangle> triangles;
	std::vector<Sphere> spheres;
	std::vector<Box> boxes;
	std::vector<Material> materials;
	std::vector<Texture> m_textureIDs;
public:
	Scene(GLuint ProgramID);
	void addSphere(Sphere s);
	void addTriangle(Triangle t);
	void addBox(Box b);
	void addMaterial(Material m);
	void addTexture(std::string filename);
	void updateSphere(int index, glm::mat4 modelMatrix);
	void render();
};

