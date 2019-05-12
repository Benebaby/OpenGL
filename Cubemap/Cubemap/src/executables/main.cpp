#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext.hpp>

#include "../Tools/ShaderSet.h"
#include "../Tools/TextureTools.h"
#include "Teapot.h"
#include "Cube.h"
#include "Camera.h"

const int width = 1600;
const int height = 900;

GLFWwindow* window;
Camera cam = Camera(width, height);

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

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(40.0f), (float)width / height, 0.01f, 100.0f);
	glm::vec4 light(5.0, 5.0, 5.0, 0.0);

	const char *skyshaderfiles[2] = { SHADERS_PATH"/skybox.vert", SHADERS_PATH"/skybox.frag" };
	ShaderSet skyboxsShaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, skyshaderfiles);
	GLuint skyboxModelMatrixID = glGetUniformLocation(skyboxsShaderProgram.getProgramID(), "modelMatrix");
	GLuint skyboxViewMatrixID = glGetUniformLocation(skyboxsShaderProgram.getProgramID(), "viewMatrix");
	GLuint skyboxProjectionMatrixID = glGetUniformLocation(skyboxsShaderProgram.getProgramID(), "projectionMatrix");

	const char *cubeskyshaderfiles[2] = { SHADERS_PATH"/cubemap.vert", SHADERS_PATH"/cubemap.frag" };
	ShaderSet cubemapShaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, cubeskyshaderfiles);
	GLuint cubemapModelMatrixID = glGetUniformLocation(cubemapShaderProgram.getProgramID(), "modelMatrix");
	GLuint cubemapViewMatrixID = glGetUniformLocation(cubemapShaderProgram.getProgramID(), "viewMatrix");
	GLuint cubemapProjectionMatrixID = glGetUniformLocation(cubemapShaderProgram.getProgramID(), "projectionMatrix");
	GLuint lightID = glGetUniformLocation(cubemapShaderProgram.getProgramID(), "light_pos");

	Cube* würfel = new Cube();
	Teapot* kanne = new Teapot(cubemapShaderProgram.getProgramID(), "copperalbedo.png", "copperroughness.png", "coppermetallic.png", "coppernormal.png");

	skyboxsShaderProgram.UseProgram();
	glUniformMatrix4fv(skyboxModelMatrixID, 1, false, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(skyboxProjectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));

	cubemapShaderProgram.UseProgram();
	glUniformMatrix4fv(cubemapModelMatrixID, 1, false, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(cubemapProjectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));


	float angle = 0.0f;
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		cam.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxsShaderProgram.UseProgram();
		glUniformMatrix4fv(skyboxViewMatrixID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		würfel->render();

		cubemapShaderProgram.UseProgram();
		glUniformMatrix4fv(cubemapViewMatrixID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		glUniform4fv(lightID, 1, glm::value_ptr(light));
		kanne->render();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}