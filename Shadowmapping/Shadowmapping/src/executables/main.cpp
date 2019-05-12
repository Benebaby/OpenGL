#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../Tools/ShaderSet.h"
#include "Cube.h"
#include "Plane.h"
#include "Camera.h"
#include "Sphere.h"
#include "ScreenQuad.h"
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
	GLFWwindow* window = glfwCreateWindow(width, height, "Shadowmapping", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 susiMatrix = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(-37.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.0f, -0.65f, 0.0f));
	glm::mat4 cubiMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -2.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width/height, 1.0f, 100.0f);
	glm::vec4 light_pos(5.0f, 5.0f, 5.0f, 1.0f);
	glm::vec3 light_col(1.0f);
	glm::vec3 light_ambient(0.3f);
	glm::vec3 mat_diffuse(1.0f, 0.0f, 0.0f);
	glm::vec3 mat_specular(1.0f);
	float mat_shininess = 100.0f;

	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(light_pos.x, light_pos.y, light_pos.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightOrthoProjectionMatrix = glm::ortho(-5.0f, 5.0f, -5.f, 5.f, 1.0f, 100.0f);
	glm::mat4 lightPerspProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);

	const char* depthmapfiles[2] = { SHADERS_PATH"/depthmap.vert", SHADERS_PATH"/depthmap.frag" };
	ShaderSet depthmapProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, depthmapfiles);

	const char* shadowmapfiles[2] = { SHADERS_PATH"/shadowmap.vert", SHADERS_PATH"/shadowmap.frag" };
	ShaderSet shadowmapProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadowmapfiles);

	GLuint shadowModelID = glGetUniformLocation(shadowmapProgram.getProgramID(), "modelMatrix");
	GLuint shadowViewID = glGetUniformLocation(shadowmapProgram.getProgramID(), "viewMatrix");
	GLuint shadowProjectionID = glGetUniformLocation(shadowmapProgram.getProgramID(), "projectionMatrix");

	const char *shaderfiles[2] = {SHADERS_PATH"/shadowPhong.vert", SHADERS_PATH"/shadowPhong.frag" };
	ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shaderfiles);

	GLuint modelID = glGetUniformLocation(shaderProgram.getProgramID(), "modelMatrix");
	GLuint viewID = glGetUniformLocation(shaderProgram.getProgramID(), "viewMatrix");
	GLuint lightViewID = glGetUniformLocation(shaderProgram.getProgramID(), "lightViewMatrix");
	GLuint lightProjectionID = glGetUniformLocation(shaderProgram.getProgramID(), "lightProjectionMatrix");
	GLuint projectionID = glGetUniformLocation(shaderProgram.getProgramID(), "projectionMatrix");

	GLuint lightID = glGetUniformLocation(shaderProgram.getProgramID(), "light.pos");
	GLuint lightcolorID = glGetUniformLocation(shaderProgram.getProgramID(), "light.col");

	GLuint diffuseID = glGetUniformLocation(shaderProgram.getProgramID(), "mat.diffuse");
	GLuint specularID = glGetUniformLocation(shaderProgram.getProgramID(), "mat.specular");
	GLuint shininessID = glGetUniformLocation(shaderProgram.getProgramID(), "mat.shininess");

	GLuint ambientID = glGetUniformLocation(shaderProgram.getProgramID(), "light_ambient");

	Mesh floor = Mesh("plane.obj");
	Sphere sphere = Sphere(glm::vec3(-2.0, 0.0, -2.0), 1.0f, 200.0);
	Cube cubi = Cube();
	ScreenQuad quad = ScreenQuad();
	Mesh susi = Mesh("suzanne.obj");

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window))
	{
		cam.update();
		if (glfwGetKey(window, GLFW_KEY_O)) {
			light_pos.w = 0.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_P)) {
			light_pos.w = 1.0f;
		}

		glViewport(0, 0, 4096, 4096);
		shadowmapProgram.UseProgram();
		glUniformMatrix4fv(shadowModelID, 1, false, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shadowViewID, 1, false, glm::value_ptr(lightViewMatrix));
		if(light_pos.w > 0.1)
			glUniformMatrix4fv(shadowProjectionID, 1, false, glm::value_ptr(lightPerspProjectionMatrix));
		else
			glUniformMatrix4fv(shadowProjectionID, 1, false, glm::value_ptr(lightOrthoProjectionMatrix));
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(shadowModelID, 1, false, glm::value_ptr(cubiMatrix));
		cubi.render();
		glUniformMatrix4fv(shadowModelID, 1, false, glm::value_ptr(susiMatrix));
		susi.render();
		glUniformMatrix4fv(shadowModelID, 1, false, glm::value_ptr(modelMatrix));
		sphere.render();
		floor.render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//depthmapProgram.UseProgram();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//quad.render();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);
		shaderProgram.UseProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniformMatrix4fv(modelID, 1, false, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		glUniformMatrix4fv(lightViewID, 1, false, glm::value_ptr(lightViewMatrix));
		if (light_pos.w > 0.1)
			glUniformMatrix4fv(lightProjectionID, 1, false, glm::value_ptr(lightPerspProjectionMatrix));
		else
			glUniformMatrix4fv(lightProjectionID, 1, false, glm::value_ptr(lightOrthoProjectionMatrix));
		glUniformMatrix4fv(projectionID, 1, false, glm::value_ptr(projectionMatrix));
		glUniform4fv(lightID, 1, glm::value_ptr(light_pos));
		glUniform3fv(lightcolorID, 1, glm::value_ptr(light_col));
		glUniform3fv(ambientID, 1, glm::value_ptr(light_ambient));
		glUniform3fv(specularID, 1, glm::value_ptr(mat_specular));
		glUniform1f(shininessID, mat_shininess);

		mat_diffuse = glm::vec3(1.0, 0.0, 0.0);
		glUniform3fv(diffuseID, 1, glm::value_ptr(mat_diffuse));
		glUniformMatrix4fv(modelID, 1, false, glm::value_ptr(cubiMatrix));
		cubi.render();
		glUniformMatrix4fv(modelID, 1, false, glm::value_ptr(susiMatrix));
		susi.render(); 
		glUniformMatrix4fv(modelID, 1, false, glm::value_ptr(modelMatrix));
		sphere.render();

		mat_diffuse = glm::vec3(0.0, 0.0, 1.0);
		glUniform3fv(diffuseID, 1, glm::value_ptr(mat_diffuse));
		floor.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}