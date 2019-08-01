#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "../Tools/ShaderSet.h"
#include "../Tools/TextureTools.h"
#include "Camera.h"
#include "Mesh.h"

const int width = 1600;
const int height = 900;
Camera cam = Camera(width, height);
GLFWwindow* window;

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.radius -= (float)(yoffset * 0.1);
	cam.radius = glm::max(cam.radius, 0.000001f);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam.Newpos = glm::vec2(xpos, ypos);
		float xAngle = (cam.Newpos.x - cam.Oldpos.x) / cam.m_width * 2 * cam.Pi;
		float yAngle = (cam.Newpos.y - cam.Oldpos.y) / cam.m_height * cam.Pi;
		cam.Angle -= glm::vec2(xAngle, yAngle);
		cam.Angle.y = glm::max(cam.Angle.y, 0.000001f);
		cam.Angle.y = glm::min(cam.Angle.y, cam.Pi);

	}
	cam.Oldpos = glm::vec2(xpos, ypos);
}

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Prophet", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(0.81f, 0.94f, 1.0f, 1.0f);

	glm::mat4 prophetMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -3.5, 0.0)), glm::vec3(0.3));
	glm::mat4 floorMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -3.5, 0.0));
	cam.update();
	glm::mat4 viewMatrix = cam.getViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 50.0f);
	glm::vec4 lightPos = glm::vec4(3.0, 5.0, 5.0, 1.0);
	glm::vec3 lightCol = glm::vec3(1.0);
	glm::vec3 lightAmbient = glm::vec3(0.3);
	glm::vec3 matDiffuse = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 matSpecular = glm::vec3(1.0);
	float matShininess = 20.0f;

	const char* pbrShaderfiles[3] = { SHADERS_PATH"pbrShader.vert", SHADERS_PATH"pbrShader.geom", SHADERS_PATH"pbrShader.frag" };
	ShaderSet pbrShaderProgram(VERTEX_SHADER_BIT | GEOMETRY_SHADER_BIT | FRAGMENT_SHADER_BIT, pbrShaderfiles);
	GLuint pbr_ModelID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "modelMatrix");
	GLuint pbr_ViewID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "viewMatrix");
	GLuint pbr_ProjectionID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "projectionMatrix");
	GLuint pbr_LightPosID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "light_pos");
	GLuint pbr_LightColID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "light_col");
	GLuint pbr_LightAmbientID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "light_ambient");
	GLuint pbr_MatShininessID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "mat_shininess");
	GLuint pbr_NormalmapUsedID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "isNormalmapUsed");
	GLuint pbr_Time = glGetUniformLocation(pbrShaderProgram.getProgramID(), "time");
	GLuint tex1ID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "basemap");
	GLuint tex2ID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "normalmap");
	GLuint tex3ID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "specularmap");
	GLuint tex4ID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "reflectivemap");
	GLuint tex5ID = glGetUniformLocation(pbrShaderProgram.getProgramID(), "cubemap");

	const char* phongShaderfiles[2] = { SHADERS_PATH"phongShader.vert", SHADERS_PATH"phongShader.frag" };
	ShaderSet phongShaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, phongShaderfiles);
	GLuint phong_ModelID = glGetUniformLocation(phongShaderProgram.getProgramID(), "modelMatrix");
	GLuint phong_ViewID = glGetUniformLocation(phongShaderProgram.getProgramID(), "viewMatrix");
	GLuint phong_ProjectionID = glGetUniformLocation(phongShaderProgram.getProgramID(), "projectionMatrix");
	GLuint phong_LightPosID = glGetUniformLocation(phongShaderProgram.getProgramID(), "light.pos");
	GLuint phong_LightColID = glGetUniformLocation(phongShaderProgram.getProgramID(), "light.col");
	GLuint phong_LightAmbientID = glGetUniformLocation(phongShaderProgram.getProgramID(), "light_ambient");
	GLuint phong_MatDifID = glGetUniformLocation(phongShaderProgram.getProgramID(), "mat.diffuse");
	GLuint phong_MatSpecID = glGetUniformLocation(phongShaderProgram.getProgramID(), "mat.specular");
	GLuint phong_MatShininessID = glGetUniformLocation(phongShaderProgram.getProgramID(), "mat.shininess");

	Mesh helmet = Mesh("helmet.obj");
	Mesh arms = Mesh("arms.obj");
	Mesh hands = Mesh("hands.obj");
	Mesh body = Mesh("body.obj");
	Mesh legs = Mesh("legs.obj");
	Mesh glass = Mesh("visor.obj");
	Mesh visorlights = Mesh("visorLights.obj");
	Mesh plane = Mesh("plane.obj");

	GLuint helmet_basemap = TextureTools::loadTexture(TEXTURE_PATH "helmet/basemap.png", true);
	GLuint helmet_normalmap = TextureTools::loadTexture(TEXTURE_PATH "helmet/normalmap.png", true);
	GLuint helmet_specular = TextureTools::loadTexture(TEXTURE_PATH "helmet/specularmap.png", true);
	GLuint helmet_reflective = TextureTools::loadTexture(TEXTURE_PATH "helmet/reflectionmap.png", true);

	GLuint arms_basemap = TextureTools::loadTexture(TEXTURE_PATH "arms/basemap.png", true);
	GLuint arms_normalmap = TextureTools::loadTexture(TEXTURE_PATH "arms/normalmap.png", true);
	GLuint arms_specular = TextureTools::loadTexture(TEXTURE_PATH "arms/specularmap.png", true);
	GLuint arms_reflective = TextureTools::loadTexture(TEXTURE_PATH "arms/reflectionmap.png", true);

	GLuint hands_basemap = TextureTools::loadTexture(TEXTURE_PATH "hands/basemap.png", true);
	GLuint hands_normalmap = TextureTools::loadTexture(TEXTURE_PATH "hands/normalmap.png", true);
	GLuint hands_specular = TextureTools::loadTexture(TEXTURE_PATH "hands/specularmap.png", true);
	GLuint hands_reflective = TextureTools::loadTexture(TEXTURE_PATH "hands/reflectionmap.png", true);

	GLuint body_basemap = TextureTools::loadTexture(TEXTURE_PATH "body/basemap.png", true);
	GLuint body_normalmap = TextureTools::loadTexture(TEXTURE_PATH "body/normalmap.png", true);
	GLuint body_specular = TextureTools::loadTexture(TEXTURE_PATH "body/specularmap.png", true);
	GLuint body_reflective = TextureTools::loadTexture(TEXTURE_PATH "body/reflectionmap.png", true);

	GLuint legs_basemap = TextureTools::loadTexture(TEXTURE_PATH "legs/basemap.png", true);
	GLuint legs_normalmap = TextureTools::loadTexture(TEXTURE_PATH "legs/normalmap.png", true);
	GLuint legs_specular = TextureTools::loadTexture(TEXTURE_PATH "legs/specularmap.png", true);
	GLuint legs_reflective = TextureTools::loadTexture(TEXTURE_PATH "legs/reflectionmap.png", true);

	GLuint glass_basemap = TextureTools::loadTexture(TEXTURE_PATH "glass/basemap.png", true);
	GLuint glass_normalmap = TextureTools::loadTexture(TEXTURE_PATH "glass/normalmap.png", true);
	GLuint glass_specular = TextureTools::loadTexture(TEXTURE_PATH "glass/specularmap.png", true);
	GLuint glass_reflective = TextureTools::loadTexture(TEXTURE_PATH "glass/reflectionmap.png", true);

	std::vector<std::string> skyFaces = { "sky/right.jpg", "sky/left.jpg", "sky/top.jpg", "sky/bottom.jpg", "sky/front.jpg","sky/back.jpg" };
	GLuint skyCubemap = TextureTools::loadCubemap(skyFaces);


	glEnable(GL_DEPTH_TEST);
	float time = -3.14159f/2.0f;
	int isNormalmapUsed = 0;
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_E) && time < 3.14159f / 2.0f) {
			time += 0.001;
		}

		cam.update();
		viewMatrix = cam.getViewMatrix();

		if (glfwGetKey(window, GLFW_KEY_N)) {
			isNormalmapUsed = 1;
		}

		if (glfwGetKey(window, GLFW_KEY_P)) {
			isNormalmapUsed = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pbrShaderProgram.UseProgram();
		glUniform1i(tex1ID, 0);
		glUniform1i(tex2ID, 1);
		glUniform1i(tex3ID, 2);
		glUniform1i(tex4ID, 3);
		glUniform1i(tex5ID, 4);
		glUniformMatrix4fv(pbr_ModelID, 1, false, value_ptr(prophetMatrix));
		glUniformMatrix4fv(pbr_ViewID, 1, false, value_ptr(viewMatrix));
		glUniformMatrix4fv(pbr_ProjectionID, 1, false, value_ptr(projectionMatrix));
		glUniform1f(pbr_MatShininessID, matShininess);
		glUniform1f(pbr_Time, time);
		glUniform1i(pbr_NormalmapUsedID, isNormalmapUsed);
		glUniform3fv(pbr_LightAmbientID, 1, value_ptr(lightAmbient));
		glUniform3fv(pbr_LightColID, 1, value_ptr(lightCol));
		glUniform4fv(pbr_LightPosID, 1, value_ptr(lightPos));
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubemap);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, helmet_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, helmet_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, helmet_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, helmet_reflective);
		helmet.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, arms_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, arms_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, arms_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, arms_reflective);
		arms.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hands_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, hands_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, hands_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, hands_reflective);
		hands.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, body_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, body_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, body_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, body_reflective);
		body.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, legs_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, legs_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, legs_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, legs_reflective);
		legs.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glass_basemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, glass_normalmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, glass_specular);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, glass_reflective);
		glass.render();

		phongShaderProgram.UseProgram();
		glUniformMatrix4fv(phong_ViewID, 1, false, value_ptr(viewMatrix));
		glUniformMatrix4fv(phong_ProjectionID, 1, false, value_ptr(projectionMatrix));
		glUniform1f(phong_MatShininessID, matShininess);
		glUniform3fv(phong_LightAmbientID, 1, value_ptr(lightAmbient));
		glUniform3fv(phong_LightColID, 1, value_ptr(lightCol));
		glUniform4fv(phong_LightPosID, 1, value_ptr(lightPos));

		matDiffuse = glm::vec3(0.60, 0.76, 0.77);
		matSpecular = glm::vec3(1.0);
		glUniformMatrix4fv(phong_ModelID, 1, false, value_ptr(prophetMatrix));
		glUniform3fv(phong_MatDifID, 1, value_ptr(matDiffuse));
		glUniform3fv(phong_MatSpecID, 1, value_ptr(matSpecular));
		visorlights.render();

		matDiffuse = glm::vec3(0.8, 0.0, 0.0);
		matSpecular = glm::vec3(1.0);
		glUniformMatrix4fv(phong_ModelID, 1, false, value_ptr(floorMatrix));
		glUniform3fv(phong_MatDifID, 1, value_ptr(matDiffuse));
		glUniform3fv(phong_MatSpecID, 1, value_ptr(matSpecular));
		plane.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}